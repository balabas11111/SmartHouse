package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.extern.log4j.Log4j2;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
@Log4j2
public class CalculatorAverageNumber implements ICalculatedEntityFieldCalculator {

	@Override
	public String getName() {
		return this.getClass().getSimpleName();
	}
	
	@Override
	public String getDescription() {
		return "Среднее значение полей поставщиков";
	}
	
	@Override
	public String getParameterDescription() {
		return "";
	}

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		Float total = 0F;
		int count = 0;
		
		for(IEntityField f : sourceEntityFields) {
			try {
				Float val = getFieldValue(f, parameter);
				
				if(val!=null) {
					total = total + val.floatValue();
					count++;
				}
				
			}catch(Exception e) {
				log.error(e.getMessage(), e);
			}
		}
		
		total = total / count;
		
		try {
			targetEntityField.setValueWithCheck(total);
		} catch (BadValueException e) {}
		
		return targetEntityField;
	}
	
	protected Float getFieldValue(IEntityField sourceField, String parameter) {
		Number val = (Number) sourceField.getValue();
		if(val==null) {
			return null;
		}
		return val.floatValue();
	}

}
