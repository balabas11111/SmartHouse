package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;
import java.util.stream.Collectors;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
public class CalculatorAverageNumber implements ICalculatedEntityFieldCalculator {

	@Getter
	private final String name = this.getClass().getSimpleName();
	
	@Getter
	private final String description = "Среднее значение полей поставщиков";
	
	@Getter
	private final String parameterDescription = "";

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		Float average = MathUtil.getAverageFloat(sourceEntityFields);
		
		if(average!=null) {
			try {
				targetEntityField.setValueWithCheck(average);
			} catch (BadValueException e) {}
		}
		
		return targetEntityField;
	}
	
	protected Float getFieldValue(IEntityField sourceField, String parameter) {
		Number val = (Number) sourceField.getValue();
		if(val==null) {
			return null;
		}
		return val.floatValue();
	}

	@Override
	public IEntityFieldValue calculateField(IEntityField targetEntityField, Collection<IEntityFieldValue> values,
			String parameter) {
		Collection<IEntityFieldValue> fieldsToProcess = values.stream().filter(val -> val.getValueAsFloat()!=null && val.getValueAsFloat().intValue() != -127 && val.getValueAsFloat().intValue() != 85).collect(Collectors.toList());
		return new EntityFieldValueNumber(targetEntityField, MathUtil.getAverageFloat(fieldsToProcess));
	}

}
