package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

@javax.persistence.Entity
public class EntityFieldEnabledValueString extends EntityFieldEnabledValue<String> implements IEntityFieldEnabledValue<String> {

	@Override
	public void setValueStr(String value) {
		this.value = value;
	}
}
