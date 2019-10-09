package com.balabas.smarthouse.server.events.listener;

import com.balabas.smarthouse.server.events.DeviceEvent;

import java.util.Collections;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.events.service.EventListenerNotificationDispatcherAbstract;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.AbstractNotification;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Message.MessageParent;
import com.balabas.smarthouse.server.notification.Message.MessageSeverity;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.REREGISTERED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;

@Component
public class DeviceEventListener extends EventListenerNotificationDispatcherAbstract<Device, DeviceEvent> {

	@Override
	public void processEvent(DeviceEvent event) {
		if(REREGISTERED.equals(event.getEventType())) {
			
			String message = "Переподключено : "+event.getTarget().getDeviceDescr();
			
			Message header = Message.builder()
					.severity(MessageSeverity.WARNING)
					.parent(MessageParent.DEVICE)
					.message(message)
					.build();
			
			dispatch(new AbstractNotification<Device>(event.getTarget(),
					header, Collections.emptyList()));
		}
		if(INITIAL_DATA_RECEIVED.equals(event.getEventType())) {
			
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
