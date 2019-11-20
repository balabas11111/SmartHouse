package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;

public interface IViewChartEntityFieldsService {

	Iterable<ViewChartEntityFields> getAll();
	ViewChartEntityFields getChartsById(Long id);
	
	ViewChartEntityFields save(ViewChartEntityFields viewChart);
	void delete(Long id);
}
