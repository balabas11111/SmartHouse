package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

@javax.persistence.Entity
public class EntityFieldEnabledValueInteger extends EntityFieldEnabledValue<Integer> implements IEntityFieldEnabledValue<Integer> {

	@Override
	public void setValueStr(String value) {
		this.value = Integer.valueOf(value);
	}
}
