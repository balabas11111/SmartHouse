package com.balabas.smarthouse.server.events.listener;

import com.balabas.smarthouse.server.events.DeviceEvent;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.events.service.EventListenerNotificationDispatcherAbstract;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.DeviceNotification;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Message.MessageSeverity;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

import static com.balabas.smarthouse.server.notification.NotificationUtility.splitNotificationParts;
import static com.balabas.smarthouse.server.notification.NotificationUtility.MSG_DEVICE_REREGISTERED;
import static com.balabas.smarthouse.server.notification.NotificationUtility.MSG_DEVICE_REGISTERED;

import static com.balabas.smarthouse.server.notification.Message.MessageSeverity.WARNING;
import static com.balabas.smarthouse.server.notification.Message.MessageSeverity.INFO;

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
		log.debug(event.toString());
		
		if (REREGISTERED.equals(event.getEventType())) {

			initDeviceTopics(event.getTarget());
			dispatch(createNotification(event, WARNING, MSG_DEVICE_REREGISTERED));
			
		} else if (INITIAL_DATA_RECEIVED.equals(event.getEventType())) {

			initDeviceTopics(event.getTarget());
			dispatch(createNotification(event, INFO, MSG_DEVICE_REGISTERED));
			
		} else if (UPDATED.equals(event.getEventType())) {

			initDeviceTopics(event.getTarget());
		}
	}
	
	private DeviceNotification createNotification(DeviceEvent event, MessageSeverity severity, String preffix) {
		Message header = new Message(severity, 
				splitNotificationParts(preffix, event.getTarget().getDeviceDescr()));

		return new DeviceNotification(event.getTarget(), header);
	}

	private void initDeviceTopics(Device device) {
		messageService.initTopicsToFromDevice(device.getDeviceId());
	}

}
