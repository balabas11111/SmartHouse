package com.balabas.smarthouse.server.entity.alarm.impl;

import java.util.function.Predicate;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.alarm.EntityFieldAlarmMarker;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.ToString;

@SuppressWarnings("rawtypes")
@Entity
@ToString
@EntityFieldAlarmMarker(target = Number.class)
public class EntityFieldMinValueAlarm extends AlarmAbstractEntityFieldNumber {

	private static String COMPARE_SEPARATOR = " меньше ";
	private static Predicate<Integer> COMPARE_PREDICATE = compRes -> compRes > 0;
	
	public EntityFieldMinValueAlarm() {
		super(COMPARE_PREDICATE, COMPARE_SEPARATOR);
	}
	
	public EntityFieldMinValueAlarm(IEntityField entityField, Number minValue){
		super(entityField, minValue, COMPARE_PREDICATE, COMPARE_SEPARATOR);
	}

}
