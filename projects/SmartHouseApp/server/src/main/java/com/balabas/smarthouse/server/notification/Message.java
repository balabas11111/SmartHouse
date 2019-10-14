package com.balabas.smarthouse.server.notification;

import lombok.Builder;
import lombok.Getter;
import lombok.ToString;

@Builder
@ToString
public class Message {

	public enum MessageSeverity{
		INFO,
		WARNING,
		ERROR
	}
	
	public enum MessageParent{
		USER,
		DEVICE,
		SERVER
	}
	
	@Getter
	private MessageSeverity severity;
	
	@Getter
	private MessageParent parent;
	
	@Getter
	private String message;

	public Message(MessageSeverity severity, MessageParent parent, String message) {
		super();
		this.severity = severity;
		this.parent = parent;
		this.message = message;
	}
	
	public Message(MessageSeverity severity, String message) {
		super();
		this.severity = severity;
		this.parent = MessageParent.DEVICE;
		this.message = message;
	}
	
	
}
