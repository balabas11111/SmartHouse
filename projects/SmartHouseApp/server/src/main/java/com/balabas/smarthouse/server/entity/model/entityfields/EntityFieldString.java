package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldString extends EntityField<String> implements IEntityField<String> {

	@JsonAlias("value")
	private String valueSt;
	
	@Override
	public void setValue(String value) {
		this.valueSt = value;
	}
	
	@Override
	protected String fromString(String value) {
		return value;
	}
	
	@Override
	public String getValue() {
		return this.valueSt;
	}

}
