package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.service.NotificationDispatcherBase;

public abstract class EventListenerNotificationDispatcherAbstract<T extends SmartHouseItem, S extends ChangedEvent<T>>
	extends NotificationDispatcherBase<T>
	implements EventListenerNotificationDispatcher<T, S>{

}
