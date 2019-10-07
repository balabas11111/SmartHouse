package com.balabas.smarthouse.server.alarm;

import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Entity;

import lombok.Getter;

@Service
public class AlarmProcessServiceEntityImpl implements AlarmProcessServiceEntity {

	@Getter
	@Autowired
	private List<AlarmRepositoryBaseValueContainer<Entity, ?>> alarmRepositories;

	@Override
	public void activateAlarms(Device device, Entity entity) {
		getAlarmRepositories().stream().forEach(repo->
			repo.activateAlarmsForItem(device, entity));
	}

	@Override
	public List<Alarm<Entity>> getALarms(Entity entity) {
		return getAlarmRepositories().stream()
			.flatMap(repo -> repo.getAlarmsForItem(entity).stream()).collect(Collectors.toList());
	}
	
	@Override
	public List<Alarm<Entity>> getActiveAlarms(Device device){
		return getAlarmRepositories().stream()
				.flatMap(repo -> repo.getActiveAlarms(device.getDeviceId()).stream())
				.collect(Collectors.toList());
	}

}
