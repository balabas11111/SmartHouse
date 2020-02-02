package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import lombok.Getter;
import lombok.Setter;

public class AlarmStateChangeEvent implements IAlarmStateChangeEvent {

	@Getter @Setter
	private Date date;
	
	@Getter @Setter
	private IAlarmV2 alarm;
	
	@Getter @Setter
	private IAlarmStateChangeAction changeAction;

	public AlarmStateChangeEvent(IAlarmV2 alarm, IAlarmStateChangeAction changeAction) {
		super();
		this.date = new Date();
		this.alarm = alarm;
		this.changeAction = changeAction;
	}

	@Override
	public boolean isSameStateAction() {
		return alarm.isSameStateAction();
	}
	
}
