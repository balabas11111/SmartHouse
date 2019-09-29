package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.service.events.EventProcessor;

public interface BotService extends EventProcessor<Device, DeviceChangedEvent> {

	void sendHtmlMessageToAllUsers(String text);

	void sendTextMessageToAllUsers(String text);

}
