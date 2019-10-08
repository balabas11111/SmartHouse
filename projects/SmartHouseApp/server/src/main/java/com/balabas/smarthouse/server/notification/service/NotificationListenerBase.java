package com.balabas.smarthouse.server.notification.service;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Notification;

public interface NotificationListenerBase<T extends SmartHouseItem> extends NotificationListener<T, Notification<T>> {

}
