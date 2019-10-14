package com.balabas.smarthouse.server.notification;

import static java.util.Collections.emptyList;

import java.util.List;

import com.balabas.smarthouse.server.model.Device;

public class DeviceNotification extends AbstractNotification<Device> {

	public DeviceNotification(Device device, Message header) {
		this(device, header, emptyList());
	}
	
	public DeviceNotification(Device device, Message header, List<Message> messages) {
		super();
		
		this.item = device;
		this.header = header;
		this.messages = messages;
	}

}
