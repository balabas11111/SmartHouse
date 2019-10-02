package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;

public interface EventProcessorBase<T extends ChangedEvent<?>> {

	 void process(T event);
}
