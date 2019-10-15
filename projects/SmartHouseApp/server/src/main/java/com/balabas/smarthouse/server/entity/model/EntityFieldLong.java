package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.exception.BadValueException;

public class EntityFieldLong extends EntityField<Long> implements IEntityField<Long> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = Long.valueOf(value);		
	}

}
