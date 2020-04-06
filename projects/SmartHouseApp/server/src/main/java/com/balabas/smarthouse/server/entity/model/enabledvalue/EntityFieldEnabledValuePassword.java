package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.Column;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValuePassword extends EntityFieldEnabledValue<String> implements IEntityFieldEnabledValue<String> {

	@Getter @Setter
	@Column(name = "valueStr")
	protected String value;
	
	@Override
	public void setValueStr(String value) {
		this.value = value;
	}
	
	@Override
	public String getValueStr() {
		return this.value;
	}

	@Override
	public Class<?> getClazz() {
		return String.class;
	}
}
