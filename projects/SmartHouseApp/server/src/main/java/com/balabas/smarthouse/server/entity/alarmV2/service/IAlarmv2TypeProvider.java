package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.List;
import java.util.Map;
import java.util.Set;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmv2TypeProvider {

	List<AlarmV2Checker> getEnabledAlarms(IItemAbstract item);
	
	IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IItemEvent event);
	
	Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors();
	
	AlarmV2Checker getAlarmV2checker(String name);
	
	Map<String, AlarmV2Checker> getAlarmv2Checkers();
	
	Map<String, IAlarmStateChangeEventProcessor> getStateChangedProcessors();
	
	AlarmV2Checker getAlarmChecker(IAlarmV2 alarm);

	List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass);
}
