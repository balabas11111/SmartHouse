package com.balabas.smarthouse.server.entity.alarm.impl;

import com.balabas.smarthouse.server.entity.alarm.AlarmAbstractEntityField;

public abstract class AlarmAbstractEntityFieldString extends AlarmAbstractEntityField<String> {

	@Override
	protected Class<String> getWatchedValueClazz() {
		return String.class;
	}
}
