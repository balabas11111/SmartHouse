package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmv2TypeProvider {

	List<AlarmV2Checker> getEnabledAlarms(IItemAbstract item);
	
	IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IAlarmStateChangeEvent event);
	
	AlarmV2Checker getAlarmV2checker(String name);
	
	Map<String, AlarmV2Checker> getAlarmv2Checkers();
	
	Map<String, IAlarmStateChangeEventProcessor> getStateChangedProcessors();
	
	void setAlarmCheckerByName(IAlarmV2 alarm);

	List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass);
}
