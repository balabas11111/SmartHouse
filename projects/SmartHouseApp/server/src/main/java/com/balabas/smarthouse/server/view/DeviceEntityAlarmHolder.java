package com.balabas.smarthouse.server.view;

import java.util.List;
import java.util.stream.Collectors;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.google.common.collect.Lists;

import lombok.Getter;

@SuppressWarnings("rawtypes")
public class DeviceEntityAlarmHolder {

	@Getter
	private IDevice device;
	
	@Getter
	private List<IEntityAlarm> alarmsWithFieldsAttached = Lists.newArrayList();
	
	@Getter
	private List<IEntityAlarm> alarmsWithAlarmDetected = Lists.newArrayList();
	
	public DeviceEntityAlarmHolder(IDevice device, List<IEntityAlarm> alarmsWithFieldsAttached, List<IEntityAlarm> alarmsWithAlarmDetected) {
		this.device = device;
		this.alarmsWithFieldsAttached = alarmsWithFieldsAttached;
		this.alarmsWithAlarmDetected = alarmsWithAlarmDetected;
	}
	
	public List<IEntityFieldAlarm> getEntityFieldAlarms() {
		return alarmsWithFieldsAttached.stream().flatMap(entityAlarm -> entityAlarm.getAlarms().stream()).collect(Collectors.toList());
	}
	
	public List<IEntityFieldAlarm> getEntityFieldAlarmsWithAlarmDetected() {
		return alarmsWithFieldsAttached.stream().flatMap(entityAlarm -> entityAlarm.getAlarms().stream())
				.filter(entityFieldAlarm -> entityFieldAlarm.isAlarmed()).collect(Collectors.toList());
	}
	
}
