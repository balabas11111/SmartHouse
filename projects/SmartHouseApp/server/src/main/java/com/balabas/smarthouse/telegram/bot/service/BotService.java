package com.balabas.smarthouse.telegram.bot.service;

import com.balabas.smarthouse.server.service.NotifyUserService;

public interface BotService extends NotifyUserService {

	void sendHtmlMessageToAllUsers(String text);
	
	void sendTextMessageToAllUsers(String text);

}
