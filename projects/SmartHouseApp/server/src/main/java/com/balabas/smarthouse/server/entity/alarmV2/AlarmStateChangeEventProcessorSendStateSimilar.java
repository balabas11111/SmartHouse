package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

@Component
public class AlarmStateChangeEventProcessorSendStateSimilar extends AlarmStateChangeEventProcessor {

	@Override
	public void processEvent(IItemEvent event) {
		super.processEvent(event);
	}

}
