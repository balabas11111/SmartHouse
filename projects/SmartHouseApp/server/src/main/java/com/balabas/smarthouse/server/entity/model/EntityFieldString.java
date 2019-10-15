package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.exception.BadValueException;

public class EntityFieldString extends EntityField<String> implements IEntityField<String> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = value;
	}

}
