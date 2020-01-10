package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Set;

public interface IEntityFieldCalculatorService {

	ICalculatedEntityFieldCalculator getCalculator(String name);
	
	Set<ICalculatedEntityFieldCalculator> getCalculators();

}
