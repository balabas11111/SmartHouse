package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntity;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.entity.behaviour.BoilerPumpsAccumulator;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityHeaterChecker extends AlarmV2CheckerAbstractEntity implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_HEATER_ENTITY;
	
	@Autowired
	private BoilerPumpsAccumulator heater;
	
	@Override
	public void process(IAlarmV2 alarm) {
		alarm.setAlarmStateByBooleanFlagWarning(checkItemValue(alarm));
	}
	
	@Override
	protected boolean checkItemValue(IAlarmV2 alarm) {
		return heater.process(this.getItemAsEntity(alarm),alarm.getParameter());
	}
	
}
