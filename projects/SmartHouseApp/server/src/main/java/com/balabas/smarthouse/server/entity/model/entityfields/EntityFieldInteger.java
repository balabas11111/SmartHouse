package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldInteger extends EntityField<Integer> implements IEntityField<Integer> {

	@Override
	public Class<Integer> getClazz() {
		return Integer.class;
	}
	
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
	public boolean setValue(Integer value) {
		boolean equals = equalsAsValue(this.valueI, value);

		if(!equals) {
			this.valueI = value;
		}
		
		return !equals;
	}

}
