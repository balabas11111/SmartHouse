package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmv2TypeProvider {

	List<Class<?>> getEnabledAlarms(IItemAbstract item);
	
	IAlarmStateChangeEventProcessor getAlarmStateChangedEventProcessor(IAlarmStateChangeEvent event);
	
	Set<IAlarmStateChangeEventProcessor> getStateChangedProcessors();
}
