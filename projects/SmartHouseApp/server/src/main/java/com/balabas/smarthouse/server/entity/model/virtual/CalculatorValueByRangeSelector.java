package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;
import java.util.Set;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
public class CalculatorValueByRangeSelector implements ICalculatedEntityFieldCalculator {

	private static final Float DEF_VAL = -2F;
	
	@Getter
	private final String name =this.getClass().getSimpleName();
	
	@Getter
	private final String description = "Фиксирование значение val если зн.поля >= min И зн.поля < max ";
	
	@Getter
	private final String parameterDescription = "[{\"min\": 0, \"max\": 30,  \"val\": 0},{\"min\": 30, \"max\": 45,  \"val\": 1},{\"min\": 45, \"max\": 800,  \"val\": 2}]";

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		if(sourceEntityFields.size()>0) {
			
			Float targetVal = MathUtil.splitByMinMaxRange(sourceEntityFields, parameter);
			
			if(isValueEnabled(targetVal, targetEntityField.getEntityFieldEnabledValues())) {
				try {
					targetEntityField.setValueWithCheck(targetVal);
				} catch (BadValueException e) {}
			}
		}
		return targetEntityField;
	}
	
	private boolean isValueEnabled(Float valueFlt, Set<IEntityFieldEnabledValue> values) {
		boolean result = !DEF_VAL.equals(valueFlt);
		
		if(result) {
			result = false;
			for(IEntityFieldEnabledValue value: values) {
				Float valueFltEn =(Float) value.getValue();
				if(Math.abs(valueFltEn - valueFlt) < 0.0001) {
					return true;
				}
			}
		}
		
		return result;
	}

	@Override
	public IEntityFieldValue calculateField(IEntityField targetEntityField, Collection<IEntityFieldValue> values,
			String parameter) {
		return new EntityFieldValueNumber(targetEntityField, MathUtil.splitByMinMaxRange(values, parameter));
	}

}
