package com.balabas.smarthouse.server.events.service;

import java.util.List;

import com.balabas.smarthouse.server.events.ChangedEvent;

public interface EventProcessorsService {

    void processEvents(List<ChangedEvent<?>> events);
}
