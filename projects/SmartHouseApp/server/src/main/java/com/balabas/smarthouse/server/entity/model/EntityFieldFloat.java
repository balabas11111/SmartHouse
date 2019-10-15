package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.exception.BadValueException;

public class EntityFieldFloat extends EntityField<Float> implements IEntityField<Float> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = Float.valueOf(value);
	}

}
