package com.balabas.smarthouse.server.entity.service;

import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IViewChartEntityFieldsRepository;
import com.balabas.smarthouse.server.view.chart.IMetrics;
import com.balabas.smarthouse.server.view.chart.Metric;

@Service
public class ViewChartsService implements IViewChartEntityFieldsService {

	@Autowired
	private IViewChartEntityFieldsRepository viewChartsRepository;
	
	@Autowired
	private IDeviceManageService deviceService;

	@SuppressWarnings("rawtypes")
	@Override
	public IMetrics getMetricsById(Long id) {
		IMetrics metrics = viewChartsRepository.findById(id).orElse(null);
		
		Set<IEntityField> fields = new LinkedHashSet<IEntityField>();
		for(IEntityField field : metrics.getEntityFields()) {
			IEntityField fieldNew = deviceService.getEntityFieldById(field.getId());
			fields.add(fieldNew);
		}
		metrics.setEntityFields(fields);
		
		return metrics;
	}

	@Override
	public Iterable<Metric> getAll() {
		return viewChartsRepository.findAll();
	}

	@Override
	public Metric save(Metric viewChart) {
		return viewChartsRepository.save(viewChart);
	}

	@Override
	public void delete(Long id) {
		viewChartsRepository.deleteById(id);
	}

	@Override
	public List<Metric> getAllAsList() {
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
		IMetrics metrics = getMetricsById(viewChartId);
		return metrics.getEntityFieldsAsMap();
	}

}
