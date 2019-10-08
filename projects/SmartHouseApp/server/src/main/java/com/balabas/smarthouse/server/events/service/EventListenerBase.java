package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;

public interface EventListenerBase<T extends ChangedEvent<?>> {

	 void processEvent(T event);
}
