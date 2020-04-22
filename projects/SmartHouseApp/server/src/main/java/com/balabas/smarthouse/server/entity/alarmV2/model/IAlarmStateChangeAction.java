package com.balabas.smarthouse.server.entity.alarmV2.model;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IAlarmStateChangeAction extends IItemAbstract{

	AlarmState getNewState();
	AlarmState getOldState();
	
	Integer getInterval();
	void setInterval(Integer interval);
	
	String getParameter();
	void setParameter(String parameter);
	
	String executeAction(IItemAbstract item);
	
	String getStringFormatted();
	void setStringFormatted(String stringFormatted);
	
	String getStringParametersFormatted();
	void setStringParametersFormatted(String stringParametersFormatted);
	
	String getTargetBeanName();
	void setTargetBeanName(String beanName);
	
	String getHint();

	boolean isDispatchIfSameState();
	void setDispatchIfSameState(boolean dispatchIfSameState);

	String getAlarmDescription(IItemAbstract item);
	
	IEntityField getTargetField();
	void setTargetField(IEntityField targetField);
	
	Long getTargetFieldId();
	void setTargetFieldId(Long id);
	
}
