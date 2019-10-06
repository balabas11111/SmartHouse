package com.balabas.smarthouse.server.notification.service;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Notification;

public interface NotificationDispatcher<T extends SmartHouseItem> {

	List<NotificationProcessorService<T>> getNotifiers();
	
	void dispatch(Notification<T> notification);
}
