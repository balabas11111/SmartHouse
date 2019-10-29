package com.balabas.smarthouse.server.entity.alarm;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.ToString;

@SuppressWarnings("rawtypes")
@Entity
@ToString
@EntityFieldAlarm(target = Number.class)
public class EntityFieldMaxValueAlarm extends AlarmAbstractEntityFieldNumber {

	public EntityFieldMaxValueAlarm() {
		super(compRes -> compRes <= 0, " > ");
	}
	
	public EntityFieldMaxValueAlarm(IEntityField entityField, Number maxValue) {
		super(entityField, maxValue, compRes -> compRes <= 0, " > ");
	}
}
