package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldString extends EntityField<String> implements IEntityField<String> {

	@Override
	public Class<String> getClazz() {
		return String.class;
	}
	
	@JsonAlias("value")
	private String valueSt;
	
	@Override
	public boolean setValue(String value) {
		boolean equals = equalsAsValue(this.valueSt, value);

		if(!equals) {
			this.valueSt = value;
		}
		
		return !equals;	

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
