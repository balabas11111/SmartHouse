package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;

@Component
public class AlarmStateChangeEventProcessorSendStateSimilar extends AlarmStateChangeEventProcessor {

	@Override
	public void process(IItemEvent event) {
		alarmStateService.removeAllStateDescription(event);
		alarmStateService.printStateDescription(event.getItem());
		super.process(event);
	}

}
