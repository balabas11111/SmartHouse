package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;

public interface BotService {

	void sendHtmlMessageToAllUsers(String text);
	
	void sendTextMessageToAllUsers(String text);
	
	void sendDeviceRegisteredEvent(DeviceChangedEvent event);

}
