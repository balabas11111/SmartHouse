package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldInteger extends EntityField<Integer> implements IEntityField<Integer> {

	@Override
	protected Integer fromString(String value) {
		return Integer.valueOf(value);
	}

}
