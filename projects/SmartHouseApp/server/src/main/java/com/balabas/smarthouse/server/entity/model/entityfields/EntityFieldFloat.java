package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldFloat extends EntityField<Float> implements IEntityField<Float> {

	@Transient
	@JsonAlias("value")
	private Float valueF;
	
	@Override
	protected Float fromString(String value) {
		return Float.valueOf(value);
	}

	@Override
	public Float getValue() {
		return valueF;
	}

	@Override
	public void setValue(Float value) {
		this.valueF = value;
	}

}
