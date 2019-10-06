package com.balabas.smarthouse.telegram.bot.notification.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.notification.service.NotificationProcessorService;
import com.balabas.smarthouse.telegram.bot.service.BotService;

@Component
public class NotificationProcessorEntity implements NotificationProcessorService<Entity>{

	@Autowired
	private BotService botService;
	
	@Override
	public void process(Notification<Entity> notification) {
		botService.sendNotification(notification);
	}

}
