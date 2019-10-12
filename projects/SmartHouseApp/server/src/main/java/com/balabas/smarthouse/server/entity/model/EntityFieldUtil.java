package com.balabas.smarthouse.server.entity.model;

import static com.balabas.smarthouse.server.entity.model.descriptor.EntityDescriptor.booleanFromString;

import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.experimental.UtilityClass;

@SuppressWarnings("rawtypes")
@UtilityClass
public class EntityFieldUtil {

	@SuppressWarnings("unchecked")
	protected static <T> T fromStringToValue(String value, Class clazz) throws BadValueException {
		T result = null;
		
		if (clazz.equals(Boolean.class)) {
			boolean valB = booleanFromString(value);
			result = (T) Boolean.valueOf(valB);
		} else
		if (clazz.equals(Integer.class)) {
			result = (T) Integer.valueOf(value);
		} else if (clazz.equals(Long.class)) {
			result = (T) Long.valueOf(value);
		} else if (clazz.equals(Float.class)) {
			result = (T) Float.valueOf(value);
		} else if (clazz.equals(String.class)) {
			result = (T) String.valueOf(value);
		} else {
			throw new BadValueException("Bad value :", value); 
		}
		
		return result;
	}
	
	protected static <T> String fromValueToString(T value, Class clazz) throws BadValueException {
		String result = null;
		
		if (clazz.equals(Boolean.class)) {
			result = ((Boolean) value).toString();
		} else
		if (clazz.equals(Integer.class)) {
			result = ((Integer) value).toString();
		} else if (clazz.equals(Long.class)) {
			result = ((Long) value).toString();
		} else if (clazz.equals(Float.class)) {
			result = ((Float) value).toString();
		} else if (clazz.equals(String.class)) {
			result = ((String) value).toString();
		} else {
			throw new BadValueException("Bad value :"); 
		}
		
		return result;
	}
	
}
