package com.balabas.smarthouse.server.events.service;

import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.service.NotificationDispatcherExecutor;

public abstract class EventProcessorNotificationDispatcherExecutor<T extends SmartHouseItem, S extends ChangedEvent<T>>
	extends NotificationDispatcherExecutor<T>
	implements EventProcessorNotificationDispatcher<T, S>{

}
