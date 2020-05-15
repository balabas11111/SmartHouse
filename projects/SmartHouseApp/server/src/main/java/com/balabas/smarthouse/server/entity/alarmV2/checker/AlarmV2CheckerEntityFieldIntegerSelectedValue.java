package com.balabas.smarthouse.server.entity.alarmV2.checker;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntityFieldNumber;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.service.IItemAlarmStateService;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldIntegerSelectedValue extends AlarmV2CheckerAbstractEntityFieldNumber
		implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_INT_VALUE;

	@Autowired
	IEntityFieldService entityFieldService;

	@Autowired
	IItemAlarmStateService alarmStateService;

	@SuppressWarnings("rawtypes")
	@Override
	public void process(IAlarmV2 alarm) {
		IEntityField entityField = getItemAsEntityField(alarm);

		int val = entityField.getValueAsFloat().intValue();

		//String param = alarm.getParameter();
		JSONObject obj = null;

		try {
			obj = new JSONObject();
		} catch (Exception e) {
			obj = new JSONObject(AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_INT_VALUE_DEFAULT);
		}

		int okVal = obj.getInt("ok");
		int warningVal = obj.getInt("warning");
		int alarmVal = obj.getInt("alarm");

		AlarmState result = null;

		if (val == okVal) {
			result = AlarmState.OK;
		} else if (val == warningVal) {
			result = AlarmState.WARNING;
		} else if (val == alarmVal) {
			result = AlarmState.ALARM;
		} else {
			result = alarm.getAlarmState();
		}

		if (!result.equals(alarm.getAlarmState())) {
			alarm.setAlarmState(result);
		}
	}

}
