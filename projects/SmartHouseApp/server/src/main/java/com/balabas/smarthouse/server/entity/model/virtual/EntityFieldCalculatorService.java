package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class EntityFieldCalculatorService implements IEntityFieldCalculatorService{

	private static final String defaultCalculatorName = "CalculatorAverageNumber";
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private Map<String, ICalculatedEntityFieldCalculator> calculators = new HashMap();

	private Set<ICalculatedEntityFieldCalculator> calcSet;
	
	@Autowired
	public void setTransformers(Set<ICalculatedEntityFieldCalculator> set) {
		calcSet = set;
		
		for(ICalculatedEntityFieldCalculator dt : calcSet) {
			calculators.put(dt.getName(), dt);
		}
		
		log.info("Total entity field calculators " + calcSet.size());
	}
	
	@Override
	public ICalculatedEntityFieldCalculator getCalculator(String name) {
		return calculators.getOrDefault(name, null);
	}

	@Override
	public Set<ICalculatedEntityFieldCalculator> getCalculators() {
		return calcSet;
	}

	@Override
	public ICalculatedEntityFieldCalculator getDefaultCalculator() {
		return getCalculator(defaultCalculatorName);
	}
	
}
