package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.processors.BoilerPumpsAccumulator;

import lombok.Getter;

@Component
public class AlarmStateChangeEventProcessorHeaterPumpsChange extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Переключить насосы в правильное положение";
	
	@Autowired
	private BoilerPumpsAccumulator heater;
	
	@Override
	public void process(IItemEvent event) {
		heater.process(getItemAsEntity(event), null);
		super.process(event);
	}

}
