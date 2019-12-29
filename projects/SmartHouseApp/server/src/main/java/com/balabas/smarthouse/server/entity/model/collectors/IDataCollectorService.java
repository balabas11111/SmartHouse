package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IDataCollectorService {
	
	void apply(IEntityField entityField);
	
	List<IDataCollector> getCollectors(IEntityField entityField);
}
