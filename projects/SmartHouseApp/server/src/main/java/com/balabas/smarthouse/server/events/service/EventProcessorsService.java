package com.balabas.smarthouse.server.events.service;

import java.util.List;

import com.balabas.smarthouse.server.events.ChangedEvent;

@SuppressWarnings("rawtypes")
public interface EventProcessorsService {

    void processEvents(List<? extends ChangedEvent> events);
}
