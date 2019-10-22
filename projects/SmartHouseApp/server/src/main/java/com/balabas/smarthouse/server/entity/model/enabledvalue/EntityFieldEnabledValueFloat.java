package com.balabas.smarthouse.server.entity.model.enabledvalue;

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

	@Override
	public String getValueStr() {
		return this.value.toString();
		
	}
}
