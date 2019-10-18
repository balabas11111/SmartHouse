package com.balabas.smarthouse.server.entity.alarm;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import com.balabas.smarthouse.server.entity.model.IEntityField;

@SuppressWarnings("rawtypes")
public abstract class AlarmAbstractEntityField<T> extends AlarmAbstract<IEntityField, T> implements IEntityFieldAlarm<T> {

	protected Class<T> clazz;
	
	@Override
	public boolean acceptsAsWatched(IEntityField entityField) {
		return clazz.isAssignableFrom(entityField.getClazz());
	}
	
	@SuppressWarnings("unchecked")
	public AlarmAbstractEntityField(){
		Type genericSuperClass = getClass().getGenericSuperclass();

	    ParameterizedType parametrizedType = null;
	    while (parametrizedType == null) {
	        if ((genericSuperClass instanceof ParameterizedType)) {
	            parametrizedType = (ParameterizedType) genericSuperClass;
	        } else {
	            genericSuperClass = ((Class<?>) genericSuperClass).getGenericSuperclass();
	        }
	    }

	    clazz = (Class<T>) parametrizedType.getActualTypeArguments()[0];
	}
}
