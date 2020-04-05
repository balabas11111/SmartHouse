package com.balabas.smarthouse.server.entity.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.implementations.BoilerPumpsAccumulator;
import com.balabas.smarthouse.server.entity.model.IEntity;

@Component
public class BoilerPumpsAccumulatorProcessor implements ItemAbstractProcessor<IEntity> {

	@Autowired
	BoilerPumpsAccumulator heater;
	
	@Override
	public boolean check(IEntity item, String parameter) {
		return heater.checkHeater(item, parameter).isStateChangeRequired();
	}

	@Override
	public void process(IEntity item, String parameter) {
		heater.executeStateChange(heater.checkHeater(item, parameter));
	}

}
