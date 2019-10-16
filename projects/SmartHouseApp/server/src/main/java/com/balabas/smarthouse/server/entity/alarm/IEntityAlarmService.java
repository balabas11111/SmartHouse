package com.balabas.smarthouse.server.entity.alarm;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;

public interface IEntityAlarmService {

	List<IAlarm> getActiveAlarms(IDevice device);

}
