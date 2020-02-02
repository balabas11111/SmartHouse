package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

public interface IAlarmStateChangeEvent {

	Date getDate();
	
	IAlarmV2 getAlarm();
	
	IAlarmStateChangeAction getChangeAction();
	
	
}
