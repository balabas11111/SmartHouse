package com.balabas.smarthouse.server.entity.alarm;

public abstract class AlarmAbstractEntityFieldBoolean extends AlarmAbstractEntityField<Boolean> {

	@Override
	protected Class<Boolean> getWatchedClazz() {
		return Boolean.class;
	}
}
