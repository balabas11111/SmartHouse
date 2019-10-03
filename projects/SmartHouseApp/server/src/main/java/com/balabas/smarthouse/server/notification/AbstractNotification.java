package com.balabas.smarthouse.server.notification;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;

public class AbstractNotification<T extends SmartHouseItem> implements Notification<T> {

	@Getter
	private T item;
	
	@Getter
	private Message header;

	@Getter
	private List<Message> messages;

	public AbstractNotification(T item, Message header, List<Message> messages) {
		super();
		this.item = item;
		this.header = header;
		this.messages = messages;
	}
	
	public AbstractNotification() {
	}

	@Override
	public boolean valid() {
		return item!=null && messages!=null && !messages.isEmpty();
	}
	
	

}
