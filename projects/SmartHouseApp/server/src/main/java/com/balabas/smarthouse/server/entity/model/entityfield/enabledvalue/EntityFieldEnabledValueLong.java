package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueLong extends EntityFieldEnabledValue<Long> implements IEntityFieldEnabledValue<Long> {

	@Getter @Setter
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
