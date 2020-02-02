package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.INameable;

public interface IAlarmStateChangeEventProcessor extends INameable {

	boolean isTarget(IAlarmStateChangeEvent event);
	void processEvent(IAlarmStateChangeEvent event);
}
