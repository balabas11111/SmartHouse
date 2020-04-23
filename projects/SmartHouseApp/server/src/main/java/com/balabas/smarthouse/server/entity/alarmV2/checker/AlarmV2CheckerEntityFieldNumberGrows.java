package com.balabas.smarthouse.server.entity.alarmV2.checker;

import java.util.Date;
import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2CheckerAbstractEntityFieldNumber;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.service.IItemAlarmStateService;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.service.IEntityFieldService;
import com.balabas.smarthouse.server.util.DateTimeUtil;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
public class AlarmV2CheckerEntityFieldNumberGrows extends AlarmV2CheckerAbstractEntityFieldNumber implements AlarmV2Checker {

	@Getter
	private final String checkerDescription = AlarmConstants.CHECKER_DESCRIPTION_ENTITY_FIELD_GROWS;
	
	private static final Long DEFAULT_TIME = 120000L;
	
	@Autowired
	IEntityFieldService entityFieldService;
	
	@Autowired
	IItemAlarmStateService alarmStateService;
	
	@SuppressWarnings("rawtypes")
	@Override
	public void process(IAlarmV2 alarm) {
		IEntityField entityField = getItemAsEntityField(alarm);
		
		Date date2 = DateTimeUtil.getDate();
		Long period = getAlarmParameterAsLongOrElse(alarm, DEFAULT_TIME);
		Date date1 = new Date(date2.getTime() - period);
		
		List<IEntityFieldValue> vals = entityFieldService.getEntityFieldValuesLessThanDates(entityField, date1, date2);
		Boolean grows = entityFieldService.isEntityFieldValuesListGrows(vals);

		AlarmState state = null;
		Emoji resultEmoji = null;
		String resultName = "";
		String diffStr ="";
		String resultDescription = null;
		String resultPref = "";
		
		float diff = MathUtil.findDiffFloat(vals);
		
		if(diff != MathUtil.DEF_VAL_FLOAT) {
			String sign =" ";

			if(grows == null) {
				sign="±";
			} else {
				sign = grows? "+" : "-";
			}
			
			diffStr = sign + MathUtil.toStringWithPrecision(diff) + " " + Optional.ofNullable(entityField.getMeasure()).orElse("");
		}
		
		if(grows == null) {
			state = AlarmState.OK;
			resultEmoji = Emoji.WAVY_DASH;
			resultPref = "Стабильно  ";
		} else {
			state = AlarmState.WARNING;
			resultEmoji = grows ? Emoji.CHART_RISE : Emoji.CHART_FALLS;
			resultPref =  grows ? Emoji.ARROW_UP.toString() + " растет " : Emoji.ARROW_DOWN.toString() + " падает ";
		}

		resultName = "Изменение : ";
		resultDescription = resultPref + diffStr + " за " + DateTimeUtil.getAsHrMinSec(period) ;
		
		alarm.setAlarmState(state);
		alarm.setViewDescriptor(new ItemAbstractDto(resultEmoji, resultName, resultDescription));
		
		alarm.setStateDescription(resultDescription);
		
		alarmStateService.putSingleStateDescription(alarm);
	}
	
	
}
