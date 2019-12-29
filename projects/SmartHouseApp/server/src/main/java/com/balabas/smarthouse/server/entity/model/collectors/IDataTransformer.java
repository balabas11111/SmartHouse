package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.Collection;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IDataTransformer {

	String getName();
	
	IEntityField transform(IEntityField field, Collection<IEntityField> entityFields);
}
