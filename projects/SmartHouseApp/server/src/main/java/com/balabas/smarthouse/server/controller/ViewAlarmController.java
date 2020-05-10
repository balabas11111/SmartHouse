package com.balabas.smarthouse.server.controller;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_CHART_DATA;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_CHART_DATA_Y;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_DATE_AFTER;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_DATE_BEFORE;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_DATETIME;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_TITLE;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_PAGE_URL;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;
import static com.balabas.smarthouse.server.controller.ControllerConstants.DEVICES_ROOT;

import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfDevice;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfEntity;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfEntityField;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.service.IAlarmV2Service;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.util.DateTimeUtil;
import com.balabas.smarthouse.server.view.chart.ChartDataSeries;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.common.collect.Lists;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.view.page.device.refresh.interval.sec:60}")
	private Long deviceViewRefreshInterval;
	
	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;
	
	@Value("${smarthouse.server.chart.legendPosition:top}")
	private String legendPosition;
	
	@Value("${smarthouse.server.view.page.chart.default.start.hr:4}")
	private Integer defaultStartHrs;

	@Autowired
	private IAlarmV2Service alarmService;
	
	@GetMapping("/virtual/null/alarms")
	public String getManagePageRedirect(Model model) {
		return "redirect:/alarms";
	}
	
	@GetMapping("/alarms")
	public String getAlarms(
			@RequestParam(name = "alarmState", required = false) String alarmState,
			@RequestParam(name = "only", required = false) boolean only, Model model) {
		
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_PAGE_TITLE, "Просмотр текущих тревог");
		model.addAttribute(ATTR_PAGE_URL, DEVICES_ROOT);
		model.addAttribute(ATTR_PAGE_DATETIME, DateTimeUtil.getDateTimeStr());
		
		if (deviceViewRefreshInterval != null && deviceViewRefreshInterval > 0) {
			model.addAttribute(ControllerConstants.ATTR_PAGE_REFRESH_INTERVAL, deviceViewRefreshInterval);
		}

		AlarmState maxState = StringUtils.isEmpty(alarmState) ? null : AlarmState.getByName(alarmState);
		
		Map<ItemType, Map<AlarmState, List<IAlarmV2>>> map =alarmService.getAlarmsAsMap(maxState, only); 
		
		model.addAttribute("itemTypes", ItemType.ITEM_TYPES_ORDERED);
		model.addAttribute("itemTypeNames", alarmService.getItemMapNames());
		model.addAttribute("alarms", map);
		model.addAttribute("alarmsIcons", alarmService.getAlarmsIconsString(maxState, only));
		
		
		return "alarms/alarmsView.html";
	}
	
	@GetMapping("/alarmsList")
	public String getAllAlarms(@RequestParam(name = "itemType", required = false) String itemType, Model model) {
		if(StringUtils.isEmpty(itemType)) {
			itemType = ItemType.DEVICE.name();
		}
		Map<String, List<IAlarmV2>> alarms = alarmService.getAlarmsGrouppedByItemTypeWithAlarmDescriptions();
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_PAGE_TITLE, "Редактирование опасности");
		model.addAttribute("itemTypes", ItemType.ITEM_TYPES_ORDERED);
		model.addAttribute("itemType", itemType);
		model.addAttribute("alarms", alarms);
		if (deviceViewRefreshInterval != null && deviceViewRefreshInterval > 0) {
			model.addAttribute(ControllerConstants.ATTR_PAGE_REFRESH_INTERVAL, deviceViewRefreshInterval);
		}
		model.addAttribute("dateTime", DateTimeUtil.getDateTimeStr());

		return "alarms/alarmsList.html";
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@GetMapping(value = "/editAlarm")
	public String editAlarm(@RequestParam(name = "id", required = true) Long id,
			@RequestParam(name = "itemType", required = false) String itemType, Model model) {

		if (id == null || (id == 0 && StringUtils.isEmpty(itemType))) {
			throw new IllegalArgumentException("New alarm should contain ItemType");
		}

		ItemType it = ItemType.getItemTypeByName(itemType);
		IAlarmV2 alarm = id > 0 ? alarmService.getAlarm(id, it) : alarmService.newAlarm(it);
		List<AlarmV2Checker> checkers = alarmService.getCheckersByTargetItemClass(alarm.getTargetItemClass());
		
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarm", alarm);
		model.addAttribute("itemType", itemType);
		model.addAttribute("currentCheckerName", alarm.getCheckerName());
		model.addAttribute("checkers", checkers);
		model.addAttribute("emojies", Lists.newArrayList(Emoji.values()));
		model.addAttribute("targets", alarmService.getEnabledAlarmAbstractTargets(alarm));
		model.addAttribute("targetFields", alarmService.getEnabledAlarmTargets(ItemType.ENTITY_FIELD));
		if (!alarm.isNew()) {
			model.addAttribute("alarmEvent", new AlarmStateChangeAction());
			model.addAttribute("alarmStates", AlarmState.getList());
			model.addAttribute("alarmStateChangedEventProcessors",
					alarmService.getAlarmStateChangedEventProcessors(alarm));
			
			Set<IAlarmStateChangeAction> actions = alarm.getActions();
			
			alarm.setActions(actions==null?new HashSet():new HashSet(actions));
		}else {
			alarm.setActions(new HashSet());
		}
		
		return "alarms/editAlarm.html";
	}
	
	@GetMapping(value = "/viewAlarm")
	public String viewAlarm(@RequestParam(name = "id", required = true) Long id,
			@RequestParam(name = ATTR_DATE_AFTER, required = false) Long afterDate,
			@RequestParam(name = ATTR_DATE_BEFORE, required = false) Long beforeDate, Model model) throws JsonProcessingException {
		
		IAlarmV2 alarm = alarmService.getAlarm(id);
		
		if(afterDate == null) {
			afterDate = getDefaultStartDate();
		}
		
		if(beforeDate == null) {
			beforeDate = DateTimeUtil.now();
		}
		
		Date date1 = new Date(afterDate);
		Date date2 = new Date(beforeDate);
		
		List<ChartDataSeries> charts = alarmService.getAlarmStatesOrdered(alarm,date1, date2);
		List<AlarmV2Checker> checkers = alarmService.getCheckersByTargetItemClass(alarm.getTargetItemClass());
		
		String chartData = (new ObjectMapper()).writeValueAsString(charts);
		String chartYLabel = "Тревоги";
		
		model.addAttribute("legendPosition", legendPosition);
		model.addAttribute(ATTR_CHART_DATA_Y, chartYLabel);
		model.addAttribute(ATTR_CHART_DATA, chartData);
		
		model.addAttribute("alarm", alarm);
		model.addAttribute("currentCheckerName", alarm.getCheckerName());
		model.addAttribute("targets", alarmService.getEnabledAlarmAbstractTargets(alarm));
		model.addAttribute("checkers", checkers);
		
		return "alarms/viewAlarm.html";
	}

	@PostMapping("/saveAlarmEvent")
	public String saveAlarmEvent(@ModelAttribute("alarmEvent") AlarmStateChangeAction action,
			@RequestParam("alarmId") Long alarmId, @RequestParam("oldStateName") String oldStateName,
			@RequestParam("newStateName") String newStateName,
			@RequestParam(name="emojiName", required = false) String emojiName,
			@RequestParam("itemType") String itemType, Model model) {

		AlarmState oldState = AlarmState.getByName(oldStateName);
		AlarmState newState = AlarmState.getByName(newStateName);

		action.setOldState(oldState);
		action.setNewState(newState);

		ItemType it = ItemType.getItemTypeByName(itemType);
		
		Emoji emoji = Emoji.getByValue(emojiName);
		
		action.setEmoji(emoji);
		
		alarmService.addAlarmStateChangeActionToAlarm(alarmId, it, action);

		return "redirect:/editAlarm?id=" + alarmId+ "&itemType=" + itemType;
	}
	
	@GetMapping("/deleteAction")
	public String deleteAlarmEvent(@ModelAttribute("alarmEvent") AlarmStateChangeAction action,
			@RequestParam("id") Long actionId,
			@RequestParam("alarmId") Long alarmId,
			@RequestParam("itemType") String itemTypeName, Model model) {
		
		ItemType itemType = ItemType.getItemTypeByName(itemTypeName);
		
		alarmService.deleteAlarmStateChangeActionFromAlarm(alarmId, itemType, actionId);
		
		return "redirect:/editAlarm?id=" + alarmId+ "&" + getItemTypeParam(itemType);
	}

	@PostMapping(value = "/saveDeviceAlarm")
	public String saveDeviceAlarm(@ModelAttribute("alarm") AlarmOfDevice alarm,
			@RequestParam(value = "targetId", required = true) Long targetId, Model model) {
		return saveAlarm(alarm, targetId);
	}

	@PostMapping(value = "/saveEntityAlarm")
	public String saveEntityAlarm(@ModelAttribute("alarm") AlarmOfEntity alarm,
			@RequestParam(value = "targetId", required = true) Long targetId, Model model) {
		return saveAlarm(alarm, targetId);
	}

	@PostMapping(value = "/saveEntityFieldAlarm")
	public String saveEntityFieldAlarm(@ModelAttribute("alarm") AlarmOfEntityField alarm,
			@RequestParam(value = "targetId", required = true) Long targetId, Model model) {
		return saveAlarm(alarm, targetId);
	}

	@GetMapping("/deleteAlarm_{id}_{itemType}")
	public String deleteEntityFieldEnabledValue(
			@PathVariable(name = "id", required = true) Long id,
			@PathVariable(name = "itemType", required = true) String itemTypeName,Model model) {
		
		IAlarmV2 alarm = alarmService.getAlarm(id, itemTypeName);
		alarmService.deleteAlarm(alarm);

		return "redirect:/alarmsList?" + getItemTypeParam(alarm);
	}

	private String saveAlarm(IAlarmV2 alarm, Long targetId) {
		alarmService.createOrUpdateAlarm(alarm, targetId);
		return "redirect:/alarmsList?" + getItemTypeParam(alarm);
	}
	
	private String getItemTypeParam(IAlarmV2 alarm) {
		return getItemTypeParam(alarm.getItemType());
	}
	
	private String getItemTypeParam(ItemType it) {
		return "itemType=" + it.name();
	}
	
	private long getDefaultStartDate() {
		return (new Date()).getTime() - defaultStartHrs*60*60*1000;
	}

}
