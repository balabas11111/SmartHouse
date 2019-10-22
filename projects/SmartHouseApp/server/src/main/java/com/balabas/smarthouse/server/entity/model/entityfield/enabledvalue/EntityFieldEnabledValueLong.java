package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

@javax.persistence.Entity
public class EntityFieldEnabledValueLong extends EntityFieldEnabledValue<Long> implements IEntityFieldEnabledValue<Long> {

	@Override
	public void setValueStr(String value) {
		this.value = Long.valueOf(value);		
	}
}
