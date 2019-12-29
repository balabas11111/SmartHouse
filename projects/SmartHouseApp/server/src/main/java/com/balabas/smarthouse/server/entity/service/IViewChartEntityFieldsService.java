package com.balabas.smarthouse.server.entity.service;

import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;

public interface IViewChartEntityFieldsService {

	Iterable<ViewChartEntityFields> getAll();
	List<ViewChartEntityFields> getAllAsList();
	
	ViewChartEntityFields getChartsById(Long id);
	
	ViewChartEntityFields save(ViewChartEntityFields viewChart);
	void delete(Long id);
	IMetrics getById(Long id);
	Map<String, List<IEntityField>> getAllAsMap(Long viewChartId);
}
