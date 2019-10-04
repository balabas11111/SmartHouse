package com.balabas.smarthouse.server.alarm;

import com.balabas.smarthouse.server.model.ValueContainer;

public interface AlarmRepositoryBaseValueContainer<T extends ValueContainer, S extends Alarm<T>> extends AlarmRepository<T, S> {


}
