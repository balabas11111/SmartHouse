package com.balabas.smarthouse.server.events.listener;

import com.balabas.smarthouse.server.events.DeviceEvent;

import java.util.Collections;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.events.service.EventListenerNotificationDispatcherAbstract;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.AbstractNotification;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Message.MessageParent;
import com.balabas.smarthouse.server.notification.Message.MessageSeverity;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.UPDATED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.REREGISTERED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;

@Log4j2
@Component
public class DeviceEventListener extends EventListenerNotificationDispatcherAbstract<Device, DeviceEvent> {

	@Autowired
	private IDeviceMessageService messageService;
	
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
			
			try {
				messageService.sendRegisteredToDevice(event.getTarget().getDeviceId());
			} catch (MqttException e) {
				log.error(e);
			}
			
			String message = "Устройство зарегистрировано : "+event.getTarget().getDeviceDescr();
			
			Message header = Message.builder()
					.severity(MessageSeverity.INFO)
					.parent(MessageParent.DEVICE)
					.message(message)
					.build();
			
			dispatch(new AbstractNotification<Device>(event.getTarget(),
					header, Collections.emptyList()));
		}
		if(UPDATED.equals(event.getEventType())) {
			
			try {
				messageService.sendRegisteredToDevice(event.getTarget().getDeviceId());
			} catch (MqttException e) {
				log.error(e);
			}
		}
	}

}
