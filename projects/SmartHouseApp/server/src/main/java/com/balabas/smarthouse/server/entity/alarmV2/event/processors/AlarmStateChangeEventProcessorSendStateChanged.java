package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;

@Component
public class AlarmStateChangeEventProcessorSendStateChanged extends AlarmStateChangeEventProcessor {

	@Override
	public void process(IItemEvent event) {
		alarmStateService.putSingleStateDescription(event);
		alarmStateService.printStateDescription(event.getItem());
		super.process(event);
	}

}
