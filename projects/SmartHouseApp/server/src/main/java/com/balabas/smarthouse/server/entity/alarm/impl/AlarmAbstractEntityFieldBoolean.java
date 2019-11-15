package com.balabas.smarthouse.server.entity.alarm.impl;

import com.balabas.smarthouse.server.entity.alarm.AlarmAbstractEntityField;

public abstract class AlarmAbstractEntityFieldBoolean extends AlarmAbstractEntityField<Boolean> {

	@Override
	protected Class<Boolean> getWatchedValueClazz() {
		return Boolean.class;
	}
}
