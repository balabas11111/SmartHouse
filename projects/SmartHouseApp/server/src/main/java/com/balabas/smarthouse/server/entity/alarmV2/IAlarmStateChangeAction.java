package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmStateChangeAction extends IItemAbstract{

	boolean accepts(AlarmState oldState, AlarmState newState);
	
	String getStringFormatted();
	void setStringFormatted(String stringFormatted);
	
	String getStringParametersFormatted();
	void setStringParametersFormatted(String stringParametersFormatted);
	
	String getTargetBeanName();
	void setTargetBeanName(String beanName);
	
	String getHint();

	boolean isDispatchIfSameState();
	void setDispatchIfSameState(boolean dispatchIfSameState);
}
