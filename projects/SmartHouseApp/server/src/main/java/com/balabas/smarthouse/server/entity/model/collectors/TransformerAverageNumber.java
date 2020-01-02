package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.Collection;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

@Component
@DataTransformer
@SuppressWarnings("rawtypes")
public class TransformerAverageNumber implements IDataTransformer {

	@Override
	public String getName() {
		return this.getClass().getSimpleName();
	}

	@SuppressWarnings("unchecked")
	@Override
	public IEntityField transform(IEntityField field, Collection<IEntityField> entityFields) {
		Float total = 0F;
		int count = 0;
		
		for(IEntityField f : entityFields) {
			try {
				Number val = (Number) f.getValue();
				total = total + val.floatValue();
				count++;
			}catch(Exception e) {}
		}
		
		total = total / count;
		
		try {
			field.setValueWithCheck(total);
		} catch (BadValueException e) {}
		
		return field;
	}

}
