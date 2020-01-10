package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface ICalculatedEntityField extends IItemAbstract {
	
	IEntityField getTargetEntityField();
	void setTargetEntityField(IEntityField entityField);
	
	Set<IEntityField> getSourceEntityFields();
	void setSourceEntityFields(Set<IEntityField> entityFields);
	
	String getCalculatorName();
	void setCalculatorName(String calculatorName);
	
	ICalculatedEntityFieldCalculator getCalculator();
	void setCalculator(ICalculatedEntityFieldCalculator calculator);
	
	boolean isImpacted(IEntityField field);
	
	void apply(IEntityField field) throws BadValueException;
}
