package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
@Component
public class AlarmV2CheckerEntityFieldDS18D20Disconnected extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Override
	public boolean check(IAlarmV2 alarm) {
		IEntityField item = (IEntityField) alarm.getItem();
		int val = getValueTmpAsInteger((IEntityField) item).intValue();
		
		return val == -127 || val == 85;
	}
	
	@Override
	public String getCheckerDescription() {
		return "Проверка подключения DS18D20";
	}

}
