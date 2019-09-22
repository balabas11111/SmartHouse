package com.balabas.smarthouse.server.service;

import java.util.List;

import com.balabas.smarthouse.server.events.ChangedEvent;

public interface DataUpdateEventService {

    void processEvents(List<ChangedEvent<?>> events);
}
