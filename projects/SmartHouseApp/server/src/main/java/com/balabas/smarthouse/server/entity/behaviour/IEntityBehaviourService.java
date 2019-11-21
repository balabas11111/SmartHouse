package com.balabas.smarthouse.server.entity.behaviour;

import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IEntityBehaviourService {

	IEntityBehaviour getEntityBehaviour(IEntity entity);
	IEntityBehaviour cacheEntityBehaviourIfFound(IEntity entity);
	
	boolean isValueCorrect(IEntity entity, String value);
}
