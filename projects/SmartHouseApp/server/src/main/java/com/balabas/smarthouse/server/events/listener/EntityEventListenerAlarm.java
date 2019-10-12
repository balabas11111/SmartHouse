package com.balabas.smarthouse.server.events.listener;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.alarm.AlarmProcessService;
import com.balabas.smarthouse.server.events.EntityEvent;
import com.balabas.smarthouse.server.events.service.EventListenerNotificationDispatcherAbstract;
import com.balabas.smarthouse.server.model.DeviceEntity;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.UPDATED;

@Component
@Log4j2
public class EntityEventListenerAlarm
		extends EventListenerNotificationDispatcherAbstract<DeviceEntity, EntityEvent> {

	@Autowired
	private AlarmProcessService<DeviceEntity> entityAlarmService;

	@Override
	public void processEvent(EntityEvent event) {
		if (INITIAL_DATA_RECEIVED.equals(event.getEventType())) {
			log.debug("Entity alarms activation");

			entityAlarmService.activateAlarms(event.getDevice(), event.getTarget());
		} else if (UPDATED.equals(event.getEventType())) {

			entityAlarmService.getALarms(event.getTarget()).forEach(alarm -> {
				dispatch(alarm.checkItemForAlarmAndReschedule());
			});
		}
	}

}
