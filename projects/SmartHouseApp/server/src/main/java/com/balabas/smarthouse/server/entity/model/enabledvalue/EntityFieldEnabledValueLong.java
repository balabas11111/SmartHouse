package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.Column;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueLong extends EntityFieldEnabledValue<Long> implements IEntityFieldEnabledValue<Long> {

	@Getter @Setter
	@Column(name = "valueLong")
	protected Long value;
	
	@Override
	public void setValueStr(String value) {
		this.value = Long.valueOf(value);		
	}
	
	@Override
	public String getValueStr() {
		return this.value.toString();
	}
}
