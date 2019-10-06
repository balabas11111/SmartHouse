package com.balabas.smarthouse.server.notification.service;

import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Notification;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
public class NotificationDispatcherExecutor<T extends SmartHouseItem> implements NotificationDispatcher<T> {

	@Autowired(required = false)
	@Getter
	private List<NotificationProcessorService<T>> notifiers;

	@Override
	public void dispatch(Notification<T> notification) {
		if (notification != null && notification.valid()) {
			log.debug("Notification :" + notification.toString());
			notifiers.stream().forEach(notifier -> notifier.process(notification));
		}
	}

}
