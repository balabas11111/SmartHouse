package com.balabas.smarthouse.server.entity.service;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IViewChartEntityFieldsRepository;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.ViewChartEntityFields;

@Service
public class ViewChartsService implements IViewChartEntityFieldsService {

	@Autowired
	private IViewChartEntityFieldsRepository viewChartsRepository;
	
	@Autowired
	private IDeviceManageService deviceService;

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

	@Override
	public void delete(Long id) {
		viewChartsRepository.deleteById(id);
	}

	@Override
	public List<ViewChartEntityFields> getAllAsList() {
		return StreamSupport.stream(
				getAll().spliterator(), false).collect(Collectors.toList());
	}

	@Override
	public IMetrics getById(Long id) {
		return viewChartsRepository.findById(id).orElse(null);
	}

	@SuppressWarnings("rawtypes")
	@Override
	public Map<String, List<IEntityField>> getAllAsMap(Long viewChartId) {
		IMetrics metrics = getChartsById(viewChartId);
		return metrics.getEntityFieldsAsMap();
	}

}
