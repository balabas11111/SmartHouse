package com.balabas.smarthouse.server.notification.service;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Notification;

public interface NotificationProcessor<T extends SmartHouseItem, S extends Notification<T>> {

	void process(S notification);
}
