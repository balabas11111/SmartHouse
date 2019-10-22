package com.balabas.smarthouse.server.entity.model;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

@Entity
public class EntityFieldLong extends EntityField<Long> implements IEntityField<Long> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = Long.valueOf(value);		
	}

}
