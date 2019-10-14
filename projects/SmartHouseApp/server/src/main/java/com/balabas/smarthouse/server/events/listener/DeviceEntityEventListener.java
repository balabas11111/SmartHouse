package com.balabas.smarthouse.server.events.listener;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.alarm.AlarmProcessService;
import com.balabas.smarthouse.server.events.EntityEvent;
import com.balabas.smarthouse.server.events.service.EventListenerNotificationDispatcherAbstract;
import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.service.IDeviceMessageService;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.UPDATED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.ADDED;

@Component
@Log4j2
public class DeviceEntityEventListener
		extends EventListenerNotificationDispatcherAbstract<DeviceEntity, EntityEvent> {

	@Autowired
	private AlarmProcessService<DeviceEntity> entityAlarmService;
	
	@Autowired
	private IDeviceMessageService messageService;
	
	@Override
	public void processEvent(EntityEvent event) {
		if (INITIAL_DATA_RECEIVED.equals(event.getEventType())) {
			log.debug("Entity alarms activation");

			entityAlarmService.activateAlarms(event.getDevice(), event.getTarget());
		} else if (UPDATED.equals(event.getEventType())) {

			entityAlarmService.getALarms(event.getTarget()).forEach(alarm -> {
				dispatch(alarm.checkItemForAlarmAndReschedule());
			});
		} else if(ADDED.equals(event.getEventType())) {
			
			messageService.initTopicsToFromDeviceEntity(event.getTarget());
		}
	}

}
