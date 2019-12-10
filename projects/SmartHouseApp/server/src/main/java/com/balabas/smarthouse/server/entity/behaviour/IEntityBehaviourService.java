package com.balabas.smarthouse.server.entity.behaviour;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityBehaviourService {

	IEntityBehaviour getEntityBehaviour(IEntity entity);
	IEntityBehaviour cacheEntityBehaviourIfFound(IEntity entity);
	
	boolean isValueCorrect(IEntity entity, String value);
	
	boolean processReceivedValueForAlarm(IEntityField entityField, String receivedValue);
}
