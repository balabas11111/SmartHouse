package com.balabas.smarthouse.server.entity.model;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

@Entity
public class EntityFieldInteger extends EntityField<Integer> implements IEntityField<Integer> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = Integer.valueOf(value);
	}

}
