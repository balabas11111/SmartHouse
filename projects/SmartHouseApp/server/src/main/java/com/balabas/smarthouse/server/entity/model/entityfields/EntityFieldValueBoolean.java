package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

import javax.persistence.Entity;

import lombok.Getter;
import lombok.NoArgsConstructor;

@Entity
@SuppressWarnings("rawtypes")
@NoArgsConstructor
public class EntityFieldValueBoolean extends EntityFieldValue implements IEntityFieldValue {
	
	@Getter
	private Boolean valueBool;

	public EntityFieldValueBoolean(IEntityField entityField, Boolean value) {
		this.entityField = entityField;
		this.valueBool = value;
		this.date = new Date();
	}
	
	@Override
	public String getValueStr() {
		return valueBool.toString();
	}

	@Override
	public Object getValue() {
		return valueBool;
	}
}
