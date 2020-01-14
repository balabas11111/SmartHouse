package com.balabas.smarthouse.server.entity.service;

import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.Metric;

@SuppressWarnings("rawtypes")
public interface IViewChartEntityFieldsService {

	Iterable<Metric> getAll();
	List<Metric> getAllAsList();
	
	IMetrics getMetricsById(Long id);
	
	Metric save(Metric viewChart);
	void delete(Long id);
	IMetrics getById(Long id);
	Map<String, List<IEntityField>> getAllAsMap(Long viewChartId);
}
