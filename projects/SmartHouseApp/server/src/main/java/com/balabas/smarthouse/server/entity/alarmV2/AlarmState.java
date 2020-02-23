package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Arrays;
import java.util.List;

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
	
	public String getName() {
		return name;
	}
	
	public String getDescription() {
		return description;
	}
	
	public String getEmojiDescription() {
		return emoji.toString() + " " +getDescription();
	}
	
	public String getFullString() {
		return emoji.toString() + " " + name + " " + getDescription();
	}
	
	public static List<AlarmState> getList() {
		return Arrays.asList(AlarmState.values());
	}

	public static AlarmState getByName(String stateName) {
		return getList().stream().filter(a -> a.name.equals(stateName)).findFirst().orElse(NO_DATA);
	}
}
