package com.balabas.smarthouse.server.entity.alarmV2.model;

import java.awt.Color;
import java.util.Arrays;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public enum AlarmState {

	NO_DATA(Emoji.INFO, "No data", "Нет данных", false, 0, Color.gray),
	OK(Emoji.CHECK_MARK, "ok", "OK", false, 1, Color.green),
	WARNING(Emoji.WARNING, "Warning", "Предупреждение", true, 2, Color.yellow),
	ALARM(Emoji.CROSS_MARK_RED, "alarm", "Ошибка", true, 3, Color.red),
	ANY(Emoji.EMPTY_EMOJI, "any", "any", false, -1, Color.white);
	
	public static final AlarmState[] ALARM_STATE_ORDER = {ALARM, WARNING, OK, NO_DATA};
	
	Emoji emoji;
	String name;
	String description;
	boolean badState;
	int valueInt;
	Color color;
	
	private AlarmState(Emoji emoji, String name, String description, boolean bad, int valueInt, Color color) {
		this.emoji = emoji;
		this.name = name;
		this.description = description;
		this.badState = bad;
		this.valueInt = valueInt;
		this.color = color;
	}
	
	public String getName() {
		return name;
	}
	
	public String getDescription() {
		return description;
	}
	
	public Emoji getEmoji() {
		return emoji;
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
		return getList().stream().filter(a -> a.name.equalsIgnoreCase(stateName)).findFirst().orElse(NO_DATA);
	}

	public static Integer getValueInt(AlarmState alarmState) {
		return alarmState.getValueInt();
	}

	public Color getColor() {
		return this.color;
	}
}
