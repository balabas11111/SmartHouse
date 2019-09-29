package com.balabas.smarthouse.server.service.events;

import java.util.List;

import com.balabas.smarthouse.server.events.ChangedEvent;

public interface EventProcessorsService {

    void processEvents(List<ChangedEvent<?>> events);
}
