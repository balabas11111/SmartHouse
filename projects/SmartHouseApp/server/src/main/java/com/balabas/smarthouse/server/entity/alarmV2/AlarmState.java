package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public enum AlarmState {

	NO_DATA(Emoji.EMPTY_EMOJI, "No data", "Нет данных", false),
	WARNING(Emoji.WARNING, "Warning", "Предупреждение", true),
	ALARM(Emoji.CROSS_MARK_RED, "alarm", "Ошибка", true),
	OK(Emoji.CHECK_MARK, "ok", "OK", false);
	
	Emoji emoji;
	String name;
	String description;
	boolean badState;
	
	private AlarmState(Emoji emoji, String name, String description, boolean bad) {
		this.emoji = emoji;
		this.name = name;
		this.description = description;
		this.badState = bad;
	}
	
	public String getDescription() {
		return description;
	}
	
	public String getEmojiDescription() {
		return emoji.toString() + " " +getDescription();
	}
}
