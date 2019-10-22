package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueFloat extends EntityFieldEnabledValue<Float> implements IEntityFieldEnabledValue<Float> {

	@Getter @Setter
	protected Float value;
	
	@Override
	public void setValueStr(String value) {
		this.value = Float.valueOf(value);
	}
}
