package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.Device;

import lombok.ToString;

@ToString(callSuper = true)
public class DeviceEvent extends ChangedEvent<Device> {

    public DeviceEvent(Device target, EventType eventType) {
        super(target, eventType);
    }

}
