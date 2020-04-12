package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Arrays;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public enum AlarmState {

	NO_DATA(Emoji.INFO, "No data", "Нет данных", false, 0),
	OK(Emoji.CHECK_MARK, "ok", "OK", false, 1),
	WARNING(Emoji.WARNING, "Warning", "Предупреждение", true, 2),
	ALARM(Emoji.CROSS_MARK_RED, "alarm", "Ошибка", true, 3);
	
	Emoji emoji;
	String name;
	String description;
	boolean badState;
	int valueInt;
	
	private AlarmState(Emoji emoji, String name, String description, boolean bad, int valueInt) {
		this.emoji = emoji;
		this.name = name;
		this.description = description;
		this.badState = bad;
		this.valueInt = valueInt;
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
	
	public Integer getValueInt() {
		return valueInt;
	}
	
	public static List<AlarmState> getList() {
		return Arrays.asList(AlarmState.values());
	}

	public static AlarmState getByName(String stateName) {
		return getList().stream().filter(a -> a.name.equals(stateName)).findFirst().orElse(NO_DATA);
	}

	public static Integer getValueInt(AlarmState alarmState) {
		return alarmState.getValueInt();
	}
}
