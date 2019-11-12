package com.balabas.smarthouse.server.entity.service;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IEntityMessageProcessor {

	void processMessage(IEntity entity, JSONObject entityMessage);
	
}
