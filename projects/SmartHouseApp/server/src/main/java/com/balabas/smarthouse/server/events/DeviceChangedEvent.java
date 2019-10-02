package com.balabas.smarthouse.server.events;

import com.balabas.smarthouse.server.model.Device;

import lombok.ToString;

@ToString(callSuper = true)
public class DeviceChangedEvent extends ChangedEvent<Device> {

    public DeviceChangedEvent(Device target, DeviceEventType eventType) {
        super(target, eventType);
    }

}
