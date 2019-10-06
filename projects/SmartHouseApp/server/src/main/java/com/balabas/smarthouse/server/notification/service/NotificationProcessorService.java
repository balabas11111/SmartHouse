package com.balabas.smarthouse.server.notification.service;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Notification;

public interface NotificationProcessorService<T extends SmartHouseItem> extends NotificationProcessor<T, Notification<T>> {

}
