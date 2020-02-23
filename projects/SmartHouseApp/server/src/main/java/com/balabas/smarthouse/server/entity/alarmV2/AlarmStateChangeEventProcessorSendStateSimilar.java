package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.stereotype.Component;

import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
public class AlarmStateChangeEventProcessorSendStateSimilar extends AlarmStateChangeEventProcessor {

	public AlarmStateChangeEventProcessorSendStateSimilar() {
		this.setName("AlarmStateChangeEventProcessorSendStateSimilar");
	}
	
	@Override
	public void processEvent(IItemEvent event) {
		String actionResult = event.getChangeAction().executeAction(event.getItem());
		log.info("processEvent AlarmStateChangeEventProcessorSendStateChanged " + event);
	}

}
