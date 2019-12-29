package com.balabas.smarthouse.server.controller;

import java.awt.Color;
import java.io.IOException;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
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

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.entity.service.IViewChartEntityFieldsService;
import com.balabas.smarthouse.server.view.chart.ChartDataSeries;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.common.collect.Lists;
import com.google.common.collect.Sets;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_ID;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_DATE_AFTER;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_DATE_BEFORE;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_CHART_DATA_Y;
import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_CHART_DATA;

import static com.balabas.smarthouse.server.controller.ControllerConstants.URL_REDIRECT_ENTITY_FIELD_FORMAT;
import static com.balabas.smarthouse.server.controller.ControllerConstants.URL_REDIRECT_VIEW_CHART_FORMAT;

import static com.balabas.smarthouse.server.controller.ControllerConstants.MSG_NEW_CHART_PREFFIX;

@Controller
@SuppressWarnings("rawtypes")
public class ViewEntityFieldController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IDeviceManageService deviceService;

	@Autowired
	private IEntityFieldService entityFieldService;

	@Autowired
	private IViewChartEntityFieldsService viewChartsService;

	@GetMapping("/entityField")
	public String getEntityField(@RequestParam(name = ATTR_ID, required = false) Long entityFieldId,
			@RequestParam(name = ATTR_DATE_AFTER, required = false) Long afterDate,
			@RequestParam(name = ATTR_DATE_BEFORE, required = false) Long beforeDate, Model model) throws IOException {

		if (entityFieldId == null || entityFieldId == 0) {
			throw new IllegalArgumentException("Bad arguments");
		}
		
		if(afterDate == null && beforeDate == null) {
			afterDate = (new Date()).getTime() - 24*60*60*1000;
			return String.format(URL_REDIRECT_ENTITY_FIELD_FORMAT, entityFieldId, afterDate);
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

		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_DATE_AFTER, afterDate);
		model.addAttribute(ATTR_DATE_BEFORE, beforeDate);
		
		model.addAttribute("device", device);
		model.addAttribute("entityField", entityField);

		model.addAttribute(ATTR_CHART_DATA_Y, chartYLabel);
		model.addAttribute(ATTR_CHART_DATA, chartData);

		return "entityFields/history.html";
	}

	@GetMapping("/listChart")
	public String getViewCharts(Model model) {

		Iterable<ViewChartEntityFields> viewCharts = viewChartsService.getAll();

		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("viewCharts", viewCharts);

		return "entityFields/listChart.html";
	}

	@GetMapping("/editChart")
	public String editChartCharts(@RequestParam(name = ATTR_ID, required = false) Long id, Model model) {

		boolean isNew = id == null || id == 0;

		IMetrics viewChart = viewChartsService.getMetricsById(id);

		Map<Long,Long> currentChartSeriesIds = new HashMap<>();
		
		Set<IEntityField> allFields = deviceService.getDevices().stream().sorted(ItemAbstract::compareByName)
				.flatMap(device -> device.getEntities().stream())
				.flatMap(entity -> entity.getEntityFields().stream())
				.filter(entityField -> ItemType.SENSORS.equals(entityField.getEntity().getItemType())
						&& !String.class.equals(entityField.getClazz())
						&& (!StringUtils.isEmpty(entityField.getMeasure()) || entityField.isButton() || Float.class.isAssignableFrom(entityField.getClazz())))
				.collect(Collectors.toCollection(LinkedHashSet::new));
		
		if (isNew) {
			viewChart = new ViewChartEntityFields();
			viewChart.setEntityFields(new HashSet<>());
		}
		
		viewChart.getEntityFields().stream().forEach( ef ->	currentChartSeriesIds.put(ef.getId(), ef.getId()));

		String pageHeader = isNew ? MSG_NEW_CHART_PREFFIX: viewChart.getName() + " " + viewChart.getDescription();
		
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("pageHeader", pageHeader);
		model.addAttribute("viewChart", viewChart);
		model.addAttribute("allFields", allFields);
		model.addAttribute("currentFields", currentChartSeriesIds);

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

		//return "redirect:/editChart?id=" + viewChart.getId();
		return "redirect:/listChart";
	}

	@GetMapping("/viewChart")
	public String getViewCharts(@RequestParam(name = ATTR_ID, required = false) Long viewChartId,
			@RequestParam(name = ATTR_DATE_AFTER, required = false) Long afterDate,
			@RequestParam(name = ATTR_DATE_BEFORE, required = false) Long beforeDate, Model model) throws IOException {

		if (viewChartId == null || viewChartId == 0) {
			throw new IllegalArgumentException("Bad arguments");
		}
		
		if(afterDate == null && beforeDate == null) {
			afterDate = (new Date()).getTime() - 24*60*60*1000;
			return String.format(URL_REDIRECT_VIEW_CHART_FORMAT, viewChartId, afterDate);
		}

		Map<String, List<IEntityField>> map = viewChartsService.getAllAsMap(viewChartId);
		
		IMetrics metrics = viewChartsService.getMetricsById(viewChartId);
		List<ChartDataSeries> charts = Lists.newArrayList();

		int colorId = 0;

		List<IEntityField> entityFields = metrics.getEntityFields().stream()
				.sorted((ef1, ef2) -> ef1.getEntity().getName().compareTo(ef2.getEntity().getName()))
				.collect(Collectors.toList());

		for (IEntityField entityField : entityFields) {
			charts.add(getSeriesforEntityField(entityField, afterDate, beforeDate, colorId));
			colorId++;
		}

		String chartData = (new ObjectMapper()).writeValueAsString(charts);
		String chartYLabel = metrics.getDescription();
		String chartHeader = metrics.getName();
		String chartId = metrics.getId().toString();

		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute(ATTR_DATE_AFTER, afterDate);
		model.addAttribute(ATTR_DATE_BEFORE, beforeDate);
		
		model.addAttribute("chartHeader", chartHeader);
		model.addAttribute("chartId", chartId);
		
		model.addAttribute(ATTR_CHART_DATA_Y, chartYLabel);
		model.addAttribute(ATTR_CHART_DATA, chartData);
		
		model.addAttribute("Metrics", map);

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

}
