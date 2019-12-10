package com.balabas.smarthouse.server.entity.behaviour;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

public interface IEntityBehaviour {

	EntityType getEntityType();
	boolean isTarget(IEntity entity);
	
	boolean isValueCorrect(IEntityFieldValue value);
	
	boolean isValueCorrect(String value);
	@SuppressWarnings("rawtypes")
	void processReceivedValueForAlarm(IEntityAlarmService alarmService, IEntityField entityField,
			String receivedValue);
}
