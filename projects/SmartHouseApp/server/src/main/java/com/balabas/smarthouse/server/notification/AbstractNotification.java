package com.balabas.smarthouse.server.notification;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;

public class AbstractNotification<T extends SmartHouseItem> implements Notification<T> {

	@Getter
	private T item;

	@Getter
	private List<Message> messages;

	public AbstractNotification(T item, List<Message> messages) {
		super();
		this.item = item;
		this.messages = messages;
	}

	@Override
	public boolean valid() {
		return item!=null && messages!=null && !messages.isEmpty();
	}
	
	

}
