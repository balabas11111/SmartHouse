package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
public class CalculatorValueByRangeSelector implements ICalculatedEntityFieldCalculator {

	private static final Float DEF_VAL = -2F;
	private static final String MAX = "max";
	private static final String MIN = "min";
	private static final String VAL = "val";
	
	@Override
	public String getName() {
		return this.getClass().getSimpleName();
	}
	
	@Override
	public String getDescription() {
		return "Фиксирование значение val если зн.поля >= min И зн.поля < max ";
	}
	
	@Override
	public String getParameterDescription() {
		return "[{\"min\": 0, \"max\": 30,  \"val\": 0},{\"min\": 30, \"max\": 45,  \"val\": 1},{\"min\": 45, \"max\": 800,  \"val\": 2}]";
	}

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		if(sourceEntityFields.size()>0) {
			Float targetValue = DEF_VAL;
			IEntityField sourceEntityField = (IEntityField)sourceEntityFields.toArray()[0];
			
			Float currentValue = (Float) sourceEntityField.getValue();
			JSONArray jsonArr = new JSONArray(parameter);
			
			for(Object obj : jsonArr) {
				JSONObject jsonObj = (JSONObject) obj;
				
				Float minValue = jsonObj.getFloat(MIN);
				Float maxValue = jsonObj.getFloat(MAX);
				Float value = jsonObj.getFloat(VAL);
				
				if(currentValue>=minValue && currentValue<maxValue) {
					targetValue = value;
				}
			}
			
			if(isValueEnabled(targetValue, targetEntityField.getEntityFieldEnabledValues())) {
				try {
					targetEntityField.setValueWithCheck(targetValue);
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

}
