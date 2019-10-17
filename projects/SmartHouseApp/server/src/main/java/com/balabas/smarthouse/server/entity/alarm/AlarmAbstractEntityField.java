package com.balabas.smarthouse.server.entity.alarm;

import java.lang.reflect.ParameterizedType;

import com.balabas.smarthouse.server.entity.model.IEntityField;

@SuppressWarnings("rawtypes")
public abstract class AlarmAbstractEntityField<T> extends AlarmAbstract<IEntityField, T> implements IEntityFieldAlarm<T> {

	private Class<T> clazz;
	
	@SuppressWarnings("unchecked")
	public AlarmAbstractEntityField() {
		ParameterizedType type = (ParameterizedType)this.getClass().getGenericSuperclass();
		clazz = (Class)type.getActualTypeArguments()[0];
	}
	
	@Override
	public boolean acceptsAsWatched(IEntityField entityField) {
		return clazz.isInstance(entityField.getClazz());
	}
}
