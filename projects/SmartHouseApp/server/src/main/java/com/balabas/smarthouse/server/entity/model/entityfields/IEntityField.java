package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
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

	void setValueWithNoCheckStr(String value);
	void setValueWithNoCheck(T value);
	
	IEntityFieldEnabledValue getEntityFieldEnabledValueByCurrentValue();
	
	void validateValue(T value) throws IllegalArgumentException;

	IEntityFieldEnabledValue getEntityFieldEnabledValue(T value);

	IEntityFieldEnabledValue getEntityFieldEnabledValueByValueStr(String value);

}
