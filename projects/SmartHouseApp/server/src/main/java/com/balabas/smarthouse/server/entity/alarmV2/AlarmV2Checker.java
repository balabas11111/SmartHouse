package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface AlarmV2Checker {

	Class<?> getItemClass();
	
	default String getName() {
		return this.getClass().getSimpleName();
	}
	
	default boolean isCorrectTarget(IItemAbstract item) {
		return isCorrectTarget(item.getClass());
	}
	
	default boolean isCorrectTarget(Class<?> clazz) {
		Class<?> targetClazz = getItemClass();
		boolean res = targetClazz.isAssignableFrom(clazz);
		return res;
	}
	
	void process(IAlarmV2 alarm);

	String getCheckerDescription();
	
	default boolean isParameterValid(String value) {
		return true;
	}
}
