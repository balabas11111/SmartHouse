package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.events.DeviceChangedEvent;
import com.balabas.smarthouse.server.events.service.EventProcessorBase;

public interface BotService extends EventProcessorBase<DeviceChangedEvent> {

	void sendHtmlMessageToAllUsers(String text);

	void sendTextMessageToAllUsers(String text);

}
