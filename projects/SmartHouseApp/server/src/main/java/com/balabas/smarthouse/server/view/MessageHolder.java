package com.balabas.smarthouse.server.view;

import java.util.List;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.google.common.collect.Lists;

import lombok.Getter;

public class MessageHolder {

	@Getter
	private Emoji emoji = Emoji.WARNING;
	@Getter
	private String deviceName;
	@Getter
	private String name;
	@Getter
	private String status;
	
	@Getter
	private List<String> messages = Lists.newArrayList();

	public MessageHolder(String deviceName, String name, String status) {
		this(deviceName, name, status, Emoji.WARNING);
	}
	
	public MessageHolder(String deviceName, String name, String status, Emoji emoji) {
		this.deviceName = deviceName;
		this.name = name;
		this.status = status;
		this.emoji = emoji;
	}
	
	public void addMessage(String message) {
		if(!StringUtils.isEmpty(message)) {
			this.messages.add(message);
		}
	}
}
