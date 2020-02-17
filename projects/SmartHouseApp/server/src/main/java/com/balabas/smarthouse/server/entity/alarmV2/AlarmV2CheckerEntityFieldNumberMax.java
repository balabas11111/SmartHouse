package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
@Component
public class AlarmV2CheckerEntityFieldNumberMax extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Override
	public boolean check(IAlarmV2 alarm) {
		IEntityField item = (IEntityField) alarm.getItem();
		boolean alarmed = getEntityFieldValueFloat(item) > getAsFloat(alarm.getParameter());
		return alarm.setAlarmStateByBooleanFlag(alarmed);
	}
	
	@Override
	public String getCheckerDescription() {
		return "Значение поля > максимального";
	}
	
}
