package com.balabas.smarthouse.server.alarm;

import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.model.Entity;

public interface AlarmProcessServiceEntity extends AlarmProcessService<Entity, EntityChangedEvent> {

}
