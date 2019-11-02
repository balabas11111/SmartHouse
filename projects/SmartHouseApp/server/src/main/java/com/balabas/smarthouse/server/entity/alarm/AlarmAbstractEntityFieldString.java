package com.balabas.smarthouse.server.entity.alarm;

public abstract class AlarmAbstractEntityFieldString extends AlarmAbstractEntityField<String> {

	@Override
	protected Class<String> getWatchedClazz() {
		return String.class;
	}
}
