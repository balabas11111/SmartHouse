package com.balabas.smarthouse.server.entity.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.repository.IViewChartEntityFieldsRepository;
import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;

@Service
public class ViewChartsService implements IViewChartEntityFieldsService {

	@Autowired
	private IViewChartEntityFieldsRepository viewChartsRepository;
	
	@Override
	public ViewChartEntityFields getChartsById(Long id) {
		return viewChartsRepository.findById(id).orElse(null);
	}

	@Override
	public Iterable<ViewChartEntityFields> getAll() {
		return viewChartsRepository.findAll();
	}

	@Override
	public ViewChartEntityFields save(ViewChartEntityFields viewChart) {
		return viewChartsRepository.save(viewChart);
	}

}
