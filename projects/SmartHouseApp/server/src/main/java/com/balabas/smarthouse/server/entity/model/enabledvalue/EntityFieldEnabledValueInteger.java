package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.Column;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueInteger extends EntityFieldEnabledValue<Integer> implements IEntityFieldEnabledValue<Integer> {

	@Getter @Setter
	@Column(name = "valueInt")
	protected Integer value;
	
	@Override
	public void setValueStr(String value) {
		this.value = Integer.valueOf(value);
	}

	@Override
	public String getValueStr() {
		return this.value.toString();
	}
}
