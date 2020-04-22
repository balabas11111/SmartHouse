package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
@SuppressWarnings("rawtypes")
public class AlarmStateChangeEventProcessorSendBooleanDataToEntityField extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Отправить данные параметра в формате (1,0, true, false) в поле цель";

	@Autowired
	private IDeviceManageService deviceService;

	@SuppressWarnings("unchecked")
	@Override
	public void process(IItemEvent event) {

		boolean targetValue = MathUtil.getAsBoolean(event.getChangeAction().getParameter());
		IEntityField targetField = event.getChangeAction().getTargetField();

		deviceService.setEntityFieldBooleanValueSendToDeviceIfNotVirtual(targetField, targetValue);

	}

}
