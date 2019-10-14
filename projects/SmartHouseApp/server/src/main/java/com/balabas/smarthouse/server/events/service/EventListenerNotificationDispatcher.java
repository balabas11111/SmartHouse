package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.service.NotificationDispatcher;

public interface EventListenerNotificationDispatcher<T extends SmartHouseItem, S extends ChangedEvent<T>>
		extends NotificationDispatcher<T>, EventListenerBase<S>{

}