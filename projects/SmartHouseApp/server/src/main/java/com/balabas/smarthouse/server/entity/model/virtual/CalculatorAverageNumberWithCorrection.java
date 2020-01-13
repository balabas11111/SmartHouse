package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
public class CalculatorAverageNumberWithCorrection extends CalculatorAverageNumber implements ICalculatedEntityFieldCalculator {
	
	@Override
	public String getDescription() {
		return "Среднее значение полей поставщиков с поправкой";
	}
	
	@Override
	public String getParameterDescription() {
		return "Значение поправки (Значение будет добавлено к значению поля)";
	}

	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		return super.calculate(targetEntityField, sourceEntityFields, parameter);
	}
	
	@Override
	protected Float getFieldValue(IEntityField sourceField, String parameter) {
		Number val = (Number) sourceField.getValue();
		if(val==null) {
			return null;
		}
		if(!StringUtils.isEmpty(parameter)) {
			try {
				Float correction = Float.valueOf(parameter);
				if(correction!=null) {
					val = val.floatValue() + correction;
				}
			}catch(Exception e) {
				return null;
			}
		}
		return val.floatValue();
	}

}
