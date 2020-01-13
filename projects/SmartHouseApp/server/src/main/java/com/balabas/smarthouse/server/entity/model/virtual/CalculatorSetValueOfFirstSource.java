package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import lombok.extern.log4j.Log4j2;

@Component
@CalculatedEntityFieldCalculator
@SuppressWarnings("rawtypes")
@Log4j2
public class CalculatorSetValueOfFirstSource implements ICalculatedEntityFieldCalculator {
	
	@Override
	public String getDescription() {
		return "Значение первого поля поставщика";
	}

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter) {
		if(sourceEntityFields!=null && sourceEntityFields.size()>0) {
			try {
				IEntityField sourceField = (IEntityField) sourceEntityFields.toArray()[0];
				Number val = (Number) sourceField.getValue();
			
				targetEntityField.setValueWithCheck(val);
			} catch (Exception e) {
				log.error(e.getMessage(), e);
			}
		}
				
		return targetEntityField;
	}

}
