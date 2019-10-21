package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.State;

public interface IStateable {

	State getState();

	void setState(State state);
}
