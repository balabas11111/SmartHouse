package com.balabas.smarthouse.server.entity.model;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

@Entity
public class EntityFieldFloat extends EntityField<Float> implements IEntityField<Float> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = Float.valueOf(value);
	}

}
