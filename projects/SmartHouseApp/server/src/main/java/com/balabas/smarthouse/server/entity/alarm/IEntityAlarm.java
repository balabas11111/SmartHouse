package com.balabas.smarthouse.server.entity.alarm;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;

@SuppressWarnings("rawtypes")
public interface IEntityAlarm extends IAlarmRepeatable<IEntity>, IUpdateable {
	
	Long getId();
	
	IDevice getDevice();
	
	IEntity getWatchedItem();
	void setWatchedItem(IEntity entity);
	
	List<IEntityFieldAlarm> getAlarms();
	void setAlarms(List<IEntityFieldAlarm> alarms);
	
	void putAlarm(IEntityFieldAlarm entityFieldAlarm);
	
}
