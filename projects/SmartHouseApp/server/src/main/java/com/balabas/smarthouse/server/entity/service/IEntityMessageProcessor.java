package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IEntityMessageProcessor {

	void processMessage(IEntity entity, String entityMessageText);
	
}
