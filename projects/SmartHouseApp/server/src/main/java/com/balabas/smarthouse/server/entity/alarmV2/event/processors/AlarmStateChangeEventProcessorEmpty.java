package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;

import lombok.Getter;

@Component
public class AlarmStateChangeEventProcessorEmpty extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Не выполнять никаких действий";
	
	@Override
	public void process(IItemEvent event) {
		super.process(event);
	}

}
