package com.balabas.smarthouse.server.entity.alarmV2.service;

import com.balabas.smarthouse.server.entity.model.IItemTypeable;

public interface IPostponedExecutorManager {

	void postponeExecution(IItemTypeable item);
	
	void executeAllPostponed();
}
