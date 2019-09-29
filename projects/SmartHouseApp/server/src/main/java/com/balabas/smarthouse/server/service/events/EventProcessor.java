package com.balabas.smarthouse.server.service.events;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.NameAble;

public interface EventProcessor<T extends NameAble, S extends ChangedEvent<T>> {

	void processEvent(S event);
}
