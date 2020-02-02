package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@AlarmMarker
@Entity
public class AlarmOfEntityFieldNumberMin extends AlarmOfEntityFieldNumber {

	@Override
	public boolean check(IItemAbstract item) {
		boolean alarmed = getEntityFieldValueFloat(item) < getValueAsFloat();
		return setAlarmStateByBooleanFlag(alarmed);
	}

}
