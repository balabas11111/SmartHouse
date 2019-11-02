package com.balabas.smarthouse.server.entity.model;

import java.util.Date;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;
import lombok.NoArgsConstructor;

@Entity
@SuppressWarnings("rawtypes")
@NoArgsConstructor
public class EntityFieldValueNumber extends EntityFieldValue implements IEntityFieldValue {
	
	@Getter
	private Float value;

	public EntityFieldValueNumber(IEntityField entityField, Float value) {
		this.entityField = entityField;
		this.value = value;
		this.date = new Date();
	}
	
	@Override
	public String getValueStr() {
		return value.toString();
	}
}
