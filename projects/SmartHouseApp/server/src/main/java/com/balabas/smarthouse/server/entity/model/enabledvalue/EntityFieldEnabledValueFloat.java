package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.Column;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueFloat extends EntityFieldEnabledValue<Float> implements IEntityFieldEnabledValue<Float> {

	@Getter @Setter
	@Column(name = "valueFlt")
	protected Float value;
	
	@Override
	public void setValueStr(String value) {
		this.value = Float.valueOf(value);
	}

	@Override
	public String getValueStr() {
		return this.value.toString();
		
	}

	@Override
	public Class<?> getClazz() {
		return Float.class;
	}
}
