package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.State;

public interface IDeviceStateChangeService {

	void stateChanged(IDevice device, State newState);
}
