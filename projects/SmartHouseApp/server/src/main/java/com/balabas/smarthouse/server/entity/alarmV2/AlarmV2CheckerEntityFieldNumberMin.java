package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldNumberMin extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_LOW;
	
	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		return getEntityFieldValueFloat(getItemAsEntityField(alarm)) < MathUtil.getAsFloat(alarm.getParameter());
	}

}
