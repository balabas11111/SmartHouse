package com.balabas.smarthouse.telegram.bot.service;

public interface BotService {

	void sendHtmlMessageToAllUsers(String text);

	void sendTextMessageToAllUsers(String text);

}
