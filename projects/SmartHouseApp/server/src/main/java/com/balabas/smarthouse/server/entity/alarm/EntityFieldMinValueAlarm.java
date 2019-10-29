package com.balabas.smarthouse.server.entity.alarm;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.ToString;

@SuppressWarnings("rawtypes")
@Entity
@ToString
@EntityFieldAlarm(target = Number.class)
public class EntityFieldMinValueAlarm extends AlarmAbstractEntityFieldNumber {

	public EntityFieldMinValueAlarm() {
		super(compRes -> compRes >= 0, " < ");
	}
	
	public EntityFieldMinValueAlarm(IEntityField entityField, Number minValue){
		super(entityField, minValue, compRes -> compRes >= 0, " < ");
	}

}
