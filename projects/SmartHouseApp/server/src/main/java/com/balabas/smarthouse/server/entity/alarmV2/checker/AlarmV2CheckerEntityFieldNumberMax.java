package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntityFieldNumber;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldNumberMax extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_HIGH;
	
	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		Float fieldValue = getEntityFieldValueFloat(getItemAsEntityField(alarm));
		Float maxValue = MathUtil.getAsFloat(alarm.getParameter());
		return  fieldValue > maxValue; 
	}
	
}
