package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

import javax.persistence.Entity;

import lombok.Getter;
import lombok.NoArgsConstructor;

@Entity
@SuppressWarnings("rawtypes")
@NoArgsConstructor
public class EntityFieldValueNumber extends EntityFieldValue implements IEntityFieldValue {
	
	@Getter
	private Float valueFlt;

	public EntityFieldValueNumber(IEntityField entityField, Float value) {
		this.entityField = entityField;
		this.valueFlt = value;
		this.date = new Date();
	}
	
	public EntityFieldValueNumber(IEntityField entityField) {
		this.entityField = entityField;
		this.valueFlt = ((Number)entityField.getValue()).floatValue();
		this.date = new Date();
	}
	
	@Override
	public String getValueStr() {
		return valueFlt.toString();
	}

	@Override
	public Object getValue() {
		return valueFlt;
	}
}
