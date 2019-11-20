package com.balabas.smarthouse.server.controller;

import java.awt.Color;
import java.io.IOException;
import java.util.Arrays;
import java.util.Date;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

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

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.entity.service.IViewChartEntityFieldsService;
import com.balabas.smarthouse.server.view.chart.ChartDataSeries;
import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.common.collect.Lists;
import com.google.common.collect.Sets;

@Controller
@SuppressWarnings("rawtypes")
public class ViewDeviceController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IEntityAlarmService alarmService;

	@Autowired
	private IEntityFieldService entityFieldService;

	@Autowired
	private IViewChartEntityFieldsService viewChartsService;

	@GetMapping("/")
	public String getRoot(Model model) {
		return "redirect:/index";
	}

	@GetMapping("/index")
	public String getDevicesIndex(Model model) {
		model.addAttribute("serverName", serverName);
		model.addAttribute("devices", deviceService.getDevices());
		return "devices/index.html";
	}

	@GetMapping("/device")
	public String getDevice(@RequestParam(name = "id", required = false) Long deviceId,
			@RequestParam(name = "name", required = false) String deviceName, Model model) {

		if (deviceId == null && StringUtils.isEmpty(deviceName)) {
			throw new IllegalArgumentException("Bad arguments");
		}

		Device device = (deviceId == null || deviceId == 0) ? deviceService.getDeviceByName(deviceName)
				: deviceService.getDeviceById(deviceId);

		if (device == null) {
			throw new IllegalArgumentException("Not found");
		}

		model.addAttribute("serverName", serverName);
		model.addAttribute("device", device);
		model.addAttribute("sensors", deviceService.getEntitiesForDevice(device.getId()));
		model.addAttribute("holder", deviceService.getValueActionHolder(device.getId()));

		return "devices/device.html";
	}

	@GetMapping("/entityField")
	public String getEntityField(@RequestParam(name = "id", required = false) Long entityFieldId,
			@RequestParam(name = "afterDate", required = false) Long afterDate,
			@RequestParam(name = "beforeDate", required = false) Long beforeDate, Model model) throws IOException {

		if (entityFieldId == null || entityFieldId == 0) {
			throw new IllegalArgumentException("Bad arguments");
		}

		IEntityField entityField = deviceService.getDevices().stream().flatMap(device -> device.getEntities().stream())
				.flatMap(entity -> entity.getEntityFields().stream()).filter(ef -> ef.getId().equals(entityFieldId))
				.findFirst().orElse(null);

		ChartDataSeries series = getSeriesforEntityField(entityField, afterDate, beforeDate, 10);

		List<ChartDataSeries> charts = Arrays.asList(series);

		String chartData = (new ObjectMapper()).writeValueAsString(charts);
		String chartYLabel = StringUtils.isEmpty(entityField.getMeasure()) ? entityField.getName()
				: entityField.getMeasure();
		IDevice device = entityField.getEntity().getDevice();

		model.addAttribute("afterDate", afterDate);
		model.addAttribute("beforeDate", beforeDate);
		model.addAttribute("serverName", serverName);
		model.addAttribute("device", device);
		model.addAttribute("entityField", entityField);

		model.addAttribute("chartDataY", chartYLabel);
		model.addAttribute("chartData", chartData);

		return "entityFields/history.html";
	}

	@GetMapping("/deleteDevice_{deviceId}")
	public String deleteDevice(@PathVariable(name = "deviceId") Long id, Model model) {
		deviceService.deleteDeviceById(id);

		return "redirect:/index";
	}

	@GetMapping("/executeAction_{deviceId}_{entityId}_{action}_")
	public String executeEntityAction(@PathVariable(name = "deviceId") Long deviceId,
			@PathVariable(name = "entityId") Long entityId, @PathVariable(name = "action") String action, Model model) {

		deviceService.sendDataToDevice(deviceId, entityId, action);

		return "redirect:/device?id=" + Long.toString(deviceId);

	}

	@GetMapping("/listChart")
	public String getViewCharts(Model model) {

		Iterable<ViewChartEntityFields> viewCharts = viewChartsService.getAll();

		model.addAttribute("serverName", serverName);
		model.addAttribute("viewCharts", viewCharts);

		return "entityFields/listChart.html";
	}

	@GetMapping("/editChart")
	public String editChartCharts(@RequestParam(name = "id", required = false) Long id, Model model) {

		boolean isNew = id == null || id == 0;

		ViewChartEntityFields viewChart = viewChartsService.getChartsById(id);

		if (isNew) {
			viewChart = new ViewChartEntityFields();

			Set<IEntityField> allFields = deviceService.getDevices().stream().sorted(ItemAbstract::compareByName)
					.flatMap(device -> device.getEntities().stream())
					.flatMap(entity -> entity.getEntityFields().stream())
					.filter(entityField -> ItemType.SENSORS.equals(entityField.getEntity().getItemType())
							&& !String.class.equals(entityField.getClazz())
							&& (!StringUtils.isEmpty(entityField.getMeasure()) || entityField.isButton()))
					.collect(Collectors.toCollection(LinkedHashSet::new));

			viewChart.setEntityFields(allFields);
		}

		String pageHeader = isNew ? "Новый график" : viewChart.getName() + " " + viewChart.getDescription();

		model.addAttribute("serverName", serverName);
		model.addAttribute("pageHeader", pageHeader);
		model.addAttribute("viewChart", viewChart);

		return "entityFields/editChart.html";
	}

	@PostMapping(value = "/saveView")
	public String addSave(@ModelAttribute("viewChart") ViewChartEntityFields viewChart,
			@RequestParam(value = "fields", required = false) long[] entityFieldIds, Model model) {

		if (entityFieldIds != null) {
			Set<IEntityField> entityFields = Sets.newHashSet();

			for (long entityFieldId : entityFieldIds) {
				Optional<IEntityField> entityField = entityFieldService.getEntityFieldById(entityFieldId);
				entityField.ifPresent(entityFields::add);
			}

			viewChart.setEntityFields(entityFields);
		}

		viewChart = viewChartsService.save(viewChart);

		return "redirect:/editChart?id=" + viewChart.getId();
	}

	@GetMapping("/viewChart")
	public String getViewCharts(@RequestParam(name = "id", required = false) Long viewChartId,
			@RequestParam(name = "afterDate", required = false) Long afterDate,
			@RequestParam(name = "beforeDate", required = false) Long beforeDate, Model model) throws IOException {

		if (viewChartId == null || viewChartId == 0) {
			throw new IllegalArgumentException("Bad arguments");
		}

		ViewChartEntityFields viewCharts = viewChartsService.getChartsById(viewChartId);
		List<ChartDataSeries> charts = Lists.newArrayList();

		int colorId = 0;

		List<IEntityField> entityFields = viewCharts.getEntityFields().stream()
				.sorted((ef1, ef2) -> ef1.getEntity().getName().compareTo(ef2.getEntity().getName()))
				.collect(Collectors.toList());

		for (IEntityField entityField : entityFields) {
			charts.add(getSeriesforEntityField(entityField, afterDate, beforeDate, colorId));
			colorId++;
		}

		String chartData = (new ObjectMapper()).writeValueAsString(charts);
		String chartYLabel = viewCharts.getDescription();
		String chartHeader = viewCharts.getName();
		String chartId = viewCharts.getId().toString();

		model.addAttribute("chartData", chartData);

		model.addAttribute("afterDate", afterDate);
		model.addAttribute("beforeDate", beforeDate);
		model.addAttribute("serverName", serverName);

		model.addAttribute("chartHeader", chartHeader);
		model.addAttribute("chartDataY", chartYLabel);
		model.addAttribute("chartId", chartId);

		return "entityFields/viewChart.html";
	}

	@GetMapping("/deleteViewChart_{viewChartId}")
	public String deleteViewChart(@PathVariable(name = "viewChartId") Long id, Model model) {
		viewChartsService.delete(id);

		return "redirect:/listChart";
	}

	private ChartDataSeries getSeriesforEntityField(IEntityField entityField, Long afterDate, Long beforeDate,
			int colorId) {
		if (entityField == null) {
			throw new IllegalArgumentException("Bad arguments");
		}

		List<EntityFieldValue> values = null;

		if (afterDate != null && beforeDate != null) {
			Date startDate = new Date(afterDate);
			Date endDate = new Date(beforeDate);

			values = entityFieldService.getEntityFieldValuesForEntityField(entityField.getId(), startDate, endDate);
		} else if (afterDate != null && beforeDate == null) {
			Date startDate = new Date(afterDate);

			values = entityFieldService.getEntityFieldValuesForEntityField(entityField.getId(), startDate);
		} else {
			values = entityFieldService.getEntityFieldValuesForEntityField(entityField.getId());
		}

		String chartDataHeader = entityField.getEntity().getDevice().getDescription() + " : "
				+ entityField.getEntity().getDescriptionByDescriptionField() + " : " + entityField.getEmoji() + " "
				+ entityField.getDescriptionByDescriptionField();

		Color color = ChartDataSeries.getColorByIndex(colorId);

		ChartDataSeries series = new ChartDataSeries(chartDataHeader, color);
		values.stream().forEach(series::addDataPoint);

		return series;
	}

	@GetMapping("/alarms")
	public String getDevicesAlarms(Model model) {
		model.addAttribute("serverName", serverName);
		List<Device> devices = deviceService.getDevices();

		model.addAttribute("devices", devices);
		model.addAttribute("alarms", alarmService.getDeviceAlarmsHoldersGroupped(devices));

		return "devices/alarms.html";
	}

	@GetMapping("/settings")
	public String getDevicesSettings(Model model) {
		model.addAttribute("serverName", serverName);
		return "devices/settings.html";
	}

}
