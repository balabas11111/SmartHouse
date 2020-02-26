package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.INameable;

public interface IAlarmStateChangeEventProcessor extends INameable {

	boolean isTarget(IItemEvent event);
	void process(IItemEvent event);
}
