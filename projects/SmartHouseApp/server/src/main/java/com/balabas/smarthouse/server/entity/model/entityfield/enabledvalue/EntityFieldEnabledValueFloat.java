package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

@javax.persistence.Entity
public class EntityFieldEnabledValueFloat extends EntityFieldEnabledValue<Float> implements IEntityFieldEnabledValue<Float> {

	@Override
	public void setValueStr(String value) {
		this.value = Float.valueOf(value);
	}
}
