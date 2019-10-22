package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueInteger extends EntityFieldEnabledValue<Integer> implements IEntityFieldEnabledValue<Integer> {

	@Getter @Setter
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
