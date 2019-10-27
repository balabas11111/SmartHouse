package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldInteger extends EntityField<Integer> implements IEntityField<Integer> {

	@Transient
	@JsonAlias("value")
	private Integer valueI;
	
	@Override
	protected Integer fromString(String value) {
		return Integer.valueOf(value);
	}

	@Override
	public Integer getValue() {
		return valueI;
	}

	@Override
	public void setValue(Integer value) {
		this.valueI = value;
	}

}
