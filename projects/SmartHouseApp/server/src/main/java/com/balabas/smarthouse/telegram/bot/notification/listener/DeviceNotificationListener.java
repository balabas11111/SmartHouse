package com.balabas.smarthouse.telegram.bot.notification.listener;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.notification.service.NotificationListenerBase;
import com.balabas.smarthouse.telegram.bot.service.BotService;

@Component
public class DeviceNotificationListener implements NotificationListenerBase<Device>{

	@Autowired
	private BotService botService;
	
	@Override
	public void processNotification(Notification<Device> notification) {
		botService.sendNotification(notification);
	}

}
