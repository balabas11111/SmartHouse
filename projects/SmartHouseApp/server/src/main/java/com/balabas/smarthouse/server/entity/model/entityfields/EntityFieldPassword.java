package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

@Entity
public class EntityFieldPassword extends EntityField<String> implements IEntityField<String> {

	@Override
	public void setValueStr(String value) throws BadValueException {
		this.value = value;
	}

}
