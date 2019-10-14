package com.balabas.smarthouse.server.notification;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;

import lombok.Getter;
import lombok.ToString;

@ToString
public class AbstractNotification<T extends SmartHouseItem> implements Notification<T> {

	@Getter
	protected T item;
	
	@Getter
	protected Message header;

	@Getter
	protected List<Message> messages;

	public AbstractNotification() {
	}
	
	public AbstractNotification(T item, Message header, List<Message> messages) {
		super();
		this.item = item;
		this.header = header;
		this.messages = messages;
	}

	@Override
	public boolean valid() {
		return item!=null && header!=null && messages!=null;
	}
	
	

}
