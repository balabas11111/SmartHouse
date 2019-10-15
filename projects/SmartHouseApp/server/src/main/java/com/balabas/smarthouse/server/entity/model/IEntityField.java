package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IEntityField<T extends Object> extends IItemAbstract {

	Class getClazz();
	
	T getValue();

	boolean isReadOnly();
	void setReadOnly(boolean readOnly);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
	
	void setValueStr(String value) throws BadValueException;
	
	void setValue(T value) throws BadValueException;

}
