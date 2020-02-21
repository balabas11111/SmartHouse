package com.balabas.smarthouse.server.controller;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;

import java.util.List;

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

import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfDevice;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfEntity;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmOfEntityField;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2Service;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IAlarmV2Service alarmService;

	@GetMapping("/alarmsList")
	public String getAllAlarms(Model model) {
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarms", alarmService.getAlarmsGrouppedByItemClassName());

		return "alarms/alarmsList.html";
	}

	@GetMapping(value = "/editAlarm")
	public String editAlarm(@RequestParam(name = "id", required = true) Long id, @RequestParam(name = "itemType", required = false) String itemType, Model model) {
		
		if(id==null || (id==0 && StringUtils.isEmpty(itemType))) {
			throw new IllegalArgumentException("New alarm should contain ItemType");
		}
		
		ItemType it = ItemType.getItemTypeByName(itemType);
		IAlarmV2 alarm = id > 0?alarmService.getAlarm(id):alarmService.newAlarm(it);
		List<AlarmV2Checker> checkers = alarmService.getCheckersByTargetItemClass(alarm.getTargetItemClass());
		
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarm", alarm);
		model.addAttribute("currentCheckerName", alarm.getCheckerName());
		model.addAttribute("checkers", checkers);
		model.addAttribute("targets", alarmService.getEnabledAlarmTargets(alarm));
		
		return "alarms/editAlarm.html";
	}
	
	
	@PostMapping(value = "/saveDeviceAlarm")
	public String saveDeviceAlarm(@ModelAttribute("alarm") AlarmOfDevice alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}
	
	@PostMapping(value = "/saveEntityAlarm")
	public String saveEntityAlarm(@ModelAttribute("alarm") AlarmOfEntity alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}
	
	@PostMapping(value = "/saveEntityFieldAlarm")
	public String saveEntityFieldAlarm(@ModelAttribute("alarm") AlarmOfEntityField alarm,
			@RequestParam(value = "itemId", required = true) Long itemId, Model model) {
		return saveAlarm(alarm, itemId);
	}
	
	@GetMapping("/deleteAlarm_{id}")
	public String deleteEntityFieldEnabledValue(@PathVariable(name = "id", required = true) Long id, Model model) {
		alarmService.deleteAlarm(id);
		
		return "redirect:/alarmsList";
	}
	
	private String saveAlarm(IAlarmV2 alarm, Long itemId) {
		alarmService.createOrUpdateAlarm(alarm, itemId);
		return "redirect:/alarmsList";
	}

}
