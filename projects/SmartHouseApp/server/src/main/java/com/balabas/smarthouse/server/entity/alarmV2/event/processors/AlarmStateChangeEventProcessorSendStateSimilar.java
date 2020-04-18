package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;

import lombok.Getter;

@Component
public class AlarmStateChangeEventProcessorSendStateSimilar extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Не спользовать - это бесполезный бин";
	
	@Override
	public void process(IItemEvent event) {
		alarmStateService.removeAllStateDescription(event);
		alarmStateService.printStateDescription(event.getItem());
		super.process(event);
	}

}
