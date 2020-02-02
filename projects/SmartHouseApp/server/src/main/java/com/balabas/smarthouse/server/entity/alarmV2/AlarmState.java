package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public enum AlarmState {

	NO_DATA(Emoji.EMPTY_EMOJI, "No data", false),
	WARNING(Emoji.WARNING, "Warning", true),
	ALARM(Emoji.CROSS_MARK_RED, "alarm", true),
	OK(Emoji.CHECK_MARK, "ok", false);
	
	Emoji emoji;
	String stateName;
	boolean badState;
	
	private AlarmState(Emoji emoji, String stateName, boolean bad) {
		this.emoji = emoji;
		this.stateName = stateName;
		this.badState = bad;
	}
}
