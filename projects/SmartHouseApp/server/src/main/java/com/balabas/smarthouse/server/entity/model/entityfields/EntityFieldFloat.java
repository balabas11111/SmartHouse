package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldFloat extends EntityField<Float> implements IEntityField<Float> {

	@Override
	protected Float fromString(String value) {
		return Float.valueOf(value);
	}

}
