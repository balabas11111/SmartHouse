package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntityFieldNumber;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldDS18D20Disconnected extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_DS18D20_DISCONNECTED;
	
	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		int tmp = getValueTmpAsInteger(getItemAsEntityField(alarm)).intValue();
		
		return !(tmp == -127 || tmp == 85);
	}
	
	@Override
	public boolean isParameterValid(String value) {
		return true;
	}

}