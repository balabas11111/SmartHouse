package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldString extends EntityField<String> implements IEntityField<String> {

	private String value;
	
	@Override
	protected void setValue(String value) {
		this.value = value;
	}
	
	@Override
	protected String fromString(String value) {
		return value;
	}
	
	@Override
	public String getValue() {
		return this.value;
	}

}
