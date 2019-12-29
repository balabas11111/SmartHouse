package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@MappedSuperclass
@SuppressWarnings("rawtypes")
public abstract class AlarmOfEntityFieldNumber extends AlarmOfEntityField {
	
	@Override
	public IItemAbstract getItem() {
		return entityField;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entityField = (EntityField) item;		
	}
	
	protected Float getValueAsFloat() {
		return Float.valueOf(getValue());
	}
	
	protected Integer getValueTmpAsInteger(IEntityField entityField) {
		return Integer.valueOf(entityField.getValueTmpAsInteger());
	}
	
	protected Float getEntityFieldValueFloat(IItemAbstract item) {
		IEntityField entityField = (IEntityField) item;
		return getEntityFieldValueFloat(entityField);
	}
	
	protected Float getEntityFieldValueFloat(IEntityField entityField) {
		return ((Number) entityField.getValue()).floatValue();
	}
	
}
