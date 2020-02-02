package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@AlarmMarker
@Entity
public class AlarmOfEntityFieldDS18D20Disconnected extends AlarmOfEntityFieldNumber {

	@Override
	public boolean check(IItemAbstract item) {
		int val = getValueAsFloat().intValue();
		
		boolean alarmed = val == -127 || val == 85;
		
		return setAlarmStateByBooleanFlag(alarmed);
	}

}
