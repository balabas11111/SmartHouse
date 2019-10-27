package com.balabas.smarthouse.server.entity.alarm;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
@Entity
public class EntityFieldMinValueAlarm extends AlarmAbstractEntityFieldNumber {

	public EntityFieldMinValueAlarm() {
		super(compRes -> compRes >= 0, " < ");
	}
	
	public EntityFieldMinValueAlarm(IEntityField entityField, Number minValue){
		super(entityField, minValue, compRes -> compRes >= 0, " < ");
	}

}
