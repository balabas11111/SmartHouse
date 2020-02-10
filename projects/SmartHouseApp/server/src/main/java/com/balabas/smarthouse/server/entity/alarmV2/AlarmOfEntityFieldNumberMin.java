package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@AlarmMarker
@Entity
public class AlarmOfEntityFieldNumberMin extends AlarmOfEntityFieldNumber {

	@Override
	public boolean check(IItemAbstract item) {
		boolean alarmed = getEntityFieldValueFloat(item) < getParameterAsFloat();
		return setAlarmStateByBooleanFlag(alarmed);
	}

	@Override
	public String getTypeDescription() {
		return "Значение поля < максимального";
	}
	
	@Override
	public boolean isParameterValid(String value) {
		return isValidFloat(value);
	}
}
