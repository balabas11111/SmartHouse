package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldLong extends EntityField<Long> implements IEntityField<Long> {

	@Override
	protected Long fromString(String value) {
		return Long.valueOf(value);
	}

}
