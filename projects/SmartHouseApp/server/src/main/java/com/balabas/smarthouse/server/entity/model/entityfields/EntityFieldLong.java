package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldLong extends EntityField<Long> implements IEntityField<Long> {

	@Override
	public Class<Long> getClazz() {
		return Long.class;
	}
	
	@Transient
	@JsonAlias("value")
	private Long valueL;
	
	@Override
	protected Long fromString(String value) {
		return Long.valueOf(value);
	}

	@Override
	public Long getValue() {
		return valueL;
	}

	@Override
	public void setValue(Long value) {
		this.valueL = value;
	}

}
