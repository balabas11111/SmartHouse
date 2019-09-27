package com.balabas.smarthouse.server.service;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;

public interface NotifyUserService {

	void sendDeviceRegisteredEventToAllUsers(DeviceChangedEvent event);
}
