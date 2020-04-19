package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.util.MathUtil;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
@Log4j2
public class CalculatorSetValueOfFirstSource implements ICalculatedEntityFieldCalculator {
	
	@Getter
	private final String description = "Значение первого поля поставщика";

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		if(sourceEntityFields!=null && sourceEntityFields.size()>0) {
			try {
				targetEntityField.setValueWithCheck(MathUtil.getFirstNonNullNumber(sourceEntityFields));
			} catch (Exception e) {
				log.error(e.getMessage(), e);
			}
		}
				
		return targetEntityField;
	}

	@Override
	public IEntityFieldValue calculateField(IEntityField targetEntityField, Collection<IEntityFieldValue> values,
			String parameter) {
		return new EntityFieldValueNumber(targetEntityField, MathUtil.getAverageFloat(values));
	}

}
