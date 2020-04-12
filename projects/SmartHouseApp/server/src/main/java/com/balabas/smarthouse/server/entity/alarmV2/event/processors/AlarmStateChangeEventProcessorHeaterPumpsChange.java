package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.processors.BoilerPumpsAccumulator;

@Component
public class AlarmStateChangeEventProcessorHeaterPumpsChange extends AlarmStateChangeEventProcessor {

	@Autowired
	private BoilerPumpsAccumulator heater;
	
	@Override
	public void process(IItemEvent event) {
		heater.process(getItemAsEntity(event), null);
		super.process(event);
	}

}
