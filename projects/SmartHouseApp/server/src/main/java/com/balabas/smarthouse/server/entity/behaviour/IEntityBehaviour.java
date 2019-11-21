package com.balabas.smarthouse.server.entity.behaviour;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

public interface IEntityBehaviour {

	EntityType getEntityType();
	boolean isTarget(IEntity entity);
	
	boolean isValueCorrect(IEntityFieldValue value);
}
