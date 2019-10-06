package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.notification.Notification;

public interface BotService {

	void sendHtmlMessageToAllUsers(String text);

	void sendTextMessageToAllUsers(String text);

	@SuppressWarnings("rawtypes")
	void sendNotification(Notification notification);

}
