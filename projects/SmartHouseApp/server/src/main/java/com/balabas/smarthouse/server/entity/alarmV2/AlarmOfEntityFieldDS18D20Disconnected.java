package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
@AlarmMarker
@Entity
public class AlarmOfEntityFieldDS18D20Disconnected extends AlarmOfEntityFieldNumber {

	@Override
	public boolean check(IItemAbstract item) {
		int val = getValueTmpAsInteger((IEntityField) item).intValue();
		
		boolean alarmed = val == -127 || val == 85;
		
		return setAlarmStateByBooleanFlag(alarmed);
	}
	
	@Override
	public String getTypeDescription() {
		return "Проверка подключения DS18D20";
	}

}
