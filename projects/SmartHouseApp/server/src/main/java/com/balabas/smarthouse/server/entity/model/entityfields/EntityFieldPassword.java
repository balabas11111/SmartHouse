package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldPassword extends EntityField<String> implements IEntityField<String> {

	@Override
	public Class<String> getClazz() {
		return String.class;
	}
	
	@Transient
	@JsonAlias("value")
	private String valueP;
	
	@Override
	protected String fromString(String value) {
		return value;
	}

	@Override
	public String getValue() {
		return valueP;
	}

	@Override
	public void setValue(String value) {
		this.valueP = value;
	}
	
	

}
