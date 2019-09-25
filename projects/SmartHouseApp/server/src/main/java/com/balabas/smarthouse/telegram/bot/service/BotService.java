package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;

public interface BotService {

	void sendMessageToAllUsers(String text);
	
	void sendDeviceRegisteredEvent(DeviceChangedEvent event);

}
