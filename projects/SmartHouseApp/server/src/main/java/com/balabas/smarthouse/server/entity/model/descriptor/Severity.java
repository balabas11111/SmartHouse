package com.balabas.smarthouse.server.entity.model.descriptor;

public enum Severity {
	INFO(Emoji.CHECK_MARK),
	WARN(Emoji.WARNING),
	ERROR(Emoji.ERROR);

	private Emoji emoji;
	
	private Severity(Emoji emoji) {
		this.emoji = emoji;
	}
	
	public Emoji getEmoji() {
		return this.emoji;
	}
}
