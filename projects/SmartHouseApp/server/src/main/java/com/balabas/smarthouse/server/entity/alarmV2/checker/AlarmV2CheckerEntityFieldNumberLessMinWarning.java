package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldNumberLessMinWarning extends AlarmV2CheckerEntityFieldNumberMin implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_LOW_WARN;
	
	@Override
	public void process(IAlarmV2 alarm) {
		alarm.setAlarmStateByBooleanFlagWarning(checkItemValue(alarm));
	}
}
