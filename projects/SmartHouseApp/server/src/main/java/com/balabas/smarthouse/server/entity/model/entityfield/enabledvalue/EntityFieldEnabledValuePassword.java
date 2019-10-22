package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValuePassword extends EntityFieldEnabledValue<String> implements IEntityFieldEnabledValue<String> {

	@Getter @Setter
	protected String value;
	
	@Override
	public void setValueStr(String value) {
		this.value = value;
	}
}
