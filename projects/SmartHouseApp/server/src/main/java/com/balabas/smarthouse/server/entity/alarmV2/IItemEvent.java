package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IItemEvent {

	Date getDate();
	
	IItemAbstract getItem();
	
	IAlarmStateChangeAction getChangeAction();
}
