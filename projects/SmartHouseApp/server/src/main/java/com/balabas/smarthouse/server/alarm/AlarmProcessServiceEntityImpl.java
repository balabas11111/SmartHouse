package com.balabas.smarthouse.server.alarm;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.events.EntityChangedEvent;
import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.notification.Notification;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.events.ChangedEvent.DeviceEventType.ADDED;
import static com.balabas.smarthouse.server.events.ChangedEvent.DeviceEventType.UPDATED;

import java.util.List;

@Service
@Log4j2
public class AlarmProcessServiceEntityImpl implements AlarmProcessServiceEntity {

	@Autowired
	private AlarmRepositoryBaseValueContainer<Entity, EntityMinMaxValueAlarm> minMaxAlarmRepository;
	
	@Override
	public void process(EntityChangedEvent event) {
		if(ADDED.equals(event.getEventType())){
			log.debug("New entity alarms activation");
			
			minMaxAlarmRepository.activateAlarmsForItem(event.getTarget());
		}else if(UPDATED.equals(event.getEventType())){
			Entity entity = event.getTarget();
			List<EntityMinMaxValueAlarm> alarms = minMaxAlarmRepository.getAlarmsForItem(entity);
			
			for(Alarm<Entity> alarm:alarms){
				Notification<Entity> not = alarm.checkItemForAlarmAndReschedule();
				
				if(not.valid()){
					log.info("Notification: " +not.toString());
				}
			}
		}
	}

}
