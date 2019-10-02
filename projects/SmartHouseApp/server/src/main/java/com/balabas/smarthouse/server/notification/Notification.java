package com.balabas.smarthouse.server.notification;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;

public interface Notification<T extends SmartHouseItem> {

	T  getItem();
	
	List<Message> getMessages();
	
	boolean valid();
}
