package com.balabas.smarthouse.server.entity.repository;

import java.util.Set;

import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.entity.model.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityFieldAlarmRepository {

	Set<IEntityFieldAlarm> getPossibleAlarms(IEntityField entityField);
	
	void attachAlarm(IEntityField entityField, IEntityFieldAlarm alarm);
}
