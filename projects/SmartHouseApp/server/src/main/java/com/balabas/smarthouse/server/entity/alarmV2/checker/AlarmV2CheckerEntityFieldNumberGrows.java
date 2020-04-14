package com.balabas.smarthouse.server.entity.alarmV2.checker;

import java.util.Date;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntityFieldNumber;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.util.DateTimeUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldNumberGrows extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_GROWS;
	
	private static final Long DEFAULT_TIME = 120000L;
	
	@Autowired
	IEntityFieldService entityFieldService;
	
	@SuppressWarnings("rawtypes")
	@Override
	public void process(IAlarmV2 alarm) {
		IEntityField entityField = getItemAsEntityField(alarm);
		
		Date date1 = DateTimeUtil.getDate();
		
		Date date2 = new Date(date1.getTime() - getAlarmParameterAsLongOrElse(alarm, DEFAULT_TIME));
		
		Boolean grows = entityFieldService.isEntityFieldValuesListGrows(entityFieldService.getEntityFieldValuesByDate(entityField, date1, date2));

		AlarmState state = null;
		
		if(grows == null) {
			state = AlarmState.OK;
		} else {
			state = grows ? AlarmState.ALARM : AlarmState.WARNING;
		}
		
		alarm.setAlarmState(state);
	}
	
}
