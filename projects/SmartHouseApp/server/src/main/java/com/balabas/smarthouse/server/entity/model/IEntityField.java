package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IEntityField<T extends Object> extends IItemAbstract {

	Class getClazz();
	
	T getValue();
	void setValue(T value) throws BadValueException;
	
	String getTemplateName();
	void setTemplateName(String templateName);
	
	boolean isReadOnly();
	void setReadOnly(boolean readOnly);
	
	Entity getEntity();
	void setEntity(Entity entity);
	
	Set<IEntityFieldEnabledValue> getEnabledValues();
	void setEnabledValues(Set<IEntityFieldEnabledValue> enabledValues);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
	
	String getValueStr();
	void setValueStr(String value) throws BadValueException;

	Object getValueObj();

	void setValueWithNoCheck(T value) throws BadValueException;
	
	IEntityFieldEnabledValue getEnabledValueByCurrentValue();
	
	String buildDataForCallBack();

	void validateValue(T value) throws BadValueException;

}
