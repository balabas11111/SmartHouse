package com.balabas.smarthouse.server.entity.processors;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityAbstractProvider {

	IEntity getEntity(IEntity entity, ItemAbstractDto item);
	
	IEntityField<Boolean> getFieldBoolean(IEntity pumps, ItemAbstractDto pumpboilerdto);
	
	IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item, ItemAbstractDto enabledValueTrue, ItemAbstractDto enabledValueFalse );

	IEntityField getFieldFloat(IEntity entity, ItemAbstractDto item);

	boolean changeEntityFieldState(IEntityField<Boolean> pumpBoiler, boolean boilerPumpRequiredState);

}
