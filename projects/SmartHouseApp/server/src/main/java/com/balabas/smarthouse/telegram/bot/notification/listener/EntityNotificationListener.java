package com.balabas.smarthouse.telegram.bot.notification.listener;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.notification.service.NotificationListenerBase;
import com.balabas.smarthouse.telegram.bot.service.BotService;

@Component
public class EntityNotificationListener implements NotificationListenerBase<Entity>{

	@Autowired
	private BotService botService;
	
	@Override
	public void processNotification(Notification<Entity> notification) {
		botService.sendNotification(notification);
	}

}
