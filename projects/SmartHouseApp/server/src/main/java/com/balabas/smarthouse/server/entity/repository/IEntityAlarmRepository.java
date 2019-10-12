package com.balabas.smarthouse.server.entity.repository;

import java.util.Set;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IEntityAlarmRepository {

	Set<IEntityAlarm> getPossibleAlarms(IEntity entityField);
	
	void attachAlarm(IEntity entity, IEntityAlarm alarm);
}
