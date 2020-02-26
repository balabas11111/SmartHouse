package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;

@SuppressWarnings("rawtypes")
@Component
public class AlarmV2CheckerEntityFieldDS18D20Disconnected extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_DS18D20_DISCONNECTED;
	
	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		IEntityField item = (IEntityField) alarm.getItem();
		int val = getValueTmpAsInteger((IEntityField) item).intValue();
		
		return !(val == -127 || val == 85);
	}
	
	@Override
	public boolean isParameterValid(String value) {
		return true;
	}

}
