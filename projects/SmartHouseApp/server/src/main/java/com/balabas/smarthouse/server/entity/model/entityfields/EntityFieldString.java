package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldString extends EntityField<String> implements IEntityField<String> {

	@Override
	protected String fromString(String value) {
		return value;
	}

}
