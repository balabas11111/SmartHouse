package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IEntityField<T extends Object> extends IItemAbstract {

	Class getClazz();
	
	T getValue();

	String getAction();
	void setAction(String action);
	
	boolean isReadOnly();
	void setReadOnly(boolean readOnly);
	
	Set<IEntityField<T>> getEnabledValues();
	void setEnabledValues(Set<IEntityField<T>> enabledValues);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
	
	void setValueStr(String value) throws BadValueException;
	
	void setValue(T value) throws BadValueException;

}
