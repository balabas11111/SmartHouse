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
public class EntityFieldMaxValueAlarm extends AlarmAbstractEntityFieldNumber {

	private static String COMPARE_SEPARATOR = " больше ";
	private static Predicate<Integer> COMPARE_PREDICATE = compRes -> compRes < 0;
	
	public EntityFieldMaxValueAlarm() {
		super(COMPARE_PREDICATE, COMPARE_SEPARATOR);
	}
	
	public EntityFieldMaxValueAlarm(IEntityField entityField, Number maxValue) {
		super(entityField, maxValue, COMPARE_PREDICATE, COMPARE_SEPARATOR);
	}

}
