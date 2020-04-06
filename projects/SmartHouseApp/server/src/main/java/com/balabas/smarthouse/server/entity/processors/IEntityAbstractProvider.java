package com.balabas.smarthouse.server.entity.processors;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityAbstractProvider {

	IEntity getEntity(IEntity entity, ItemAbstractDto item);
	
	IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item);

	IEntityField getFieldFloat(IEntity entity, ItemAbstractDto item);
}
