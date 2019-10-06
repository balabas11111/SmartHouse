package com.balabas.smarthouse.server.events.processors;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;

import java.util.Collections;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.events.ChangedEvent.DeviceEventType;
import com.balabas.smarthouse.server.events.service.EventProcessorNotificationDispatcherExecutor;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.AbstractNotification;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Message.MessageParent;
import com.balabas.smarthouse.server.notification.Message.MessageSeverity;

@Component
public class DeviceEventProcessor extends EventProcessorNotificationDispatcherExecutor<Device, DeviceChangedEvent> {

	@Override
	public void process(DeviceChangedEvent event) {
		if(DeviceEventType.ADDED.equals(event.getEventType())) {
			
			String message = "Устройство зарегистрировано : "+event.getTarget().getDeviceDescr();
			
			Message header = Message.builder()
					.severity(MessageSeverity.INFO)
					.parent(MessageParent.DEVICE)
					.message(message)
					.build();
			
			dispatch(new AbstractNotification<Device>(event.getTarget(),
					header, Collections.emptyList()));
		}
	}

}
