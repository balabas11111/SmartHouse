package com.balabas.smarthouse.server.alarm;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.service.EventProcessorBase;
import com.balabas.smarthouse.server.model.ValueContainer;

public interface AlarmProcessService<T extends ValueContainer, S extends ChangedEvent<T>> extends EventProcessorBase<S> {
	
}
