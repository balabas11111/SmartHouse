package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.SmartHouseItem;

public interface EventProcessor<T extends SmartHouseItem, S extends ChangedEvent<T>> {

	void processEvent(S event);
}
