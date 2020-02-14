package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface AlarmV2Checker {

	Class<?> getItemClass();
	
	default boolean isCorrectTarget(IItemAbstract item) {
		return getItemClass().isAssignableFrom(item.getClass());
	}
	
	boolean check(IAlarmV2 alarm);

	String getCheckerDescription();
	
	default boolean isParameterValid(String value) {
		return true;
	}
}
