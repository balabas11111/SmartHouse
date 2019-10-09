package com.balabas.smarthouse.server.service;

import com.balabas.smarthouse.server.events.DeviceEvent;

public interface NotifyUserService {

	void sendDeviceRegisteredEventToAllUsers(DeviceEvent event);
}
