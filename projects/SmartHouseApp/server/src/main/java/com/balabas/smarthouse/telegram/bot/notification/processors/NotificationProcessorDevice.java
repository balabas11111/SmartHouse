package com.balabas.smarthouse.telegram.bot.notification.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.server.notification.service.NotificationProcessorService;
import com.balabas.smarthouse.telegram.bot.service.BotService;

@Component
public class NotificationProcessorDevice implements NotificationProcessorService<Device>{

	@Autowired
	private BotService botService;
	
	@Override
	public void process(Notification<Device> notification) {
		botService.sendNotification(notification);
	}

}
