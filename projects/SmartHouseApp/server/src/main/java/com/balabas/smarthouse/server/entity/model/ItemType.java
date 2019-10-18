package com.balabas.smarthouse.server.entity.model;

public enum ItemType {
	DEVICE(1000 * 60 * 2, "Устройство", Emoji.PAGER),
	SENSORS(1000 * 60 * 1, "Датчики", Emoji.JAPANESE_SYMBOL_FOR_BEGINNER),
	SETTINGS(1000 * 60 * 5, "Настройки", Emoji.HAMMER_AND_WRENCH),
	SERVICES(1000 * 60 * 2, "Сервисы", Emoji.BRIEFCASE),
	CUSTOM(-1, "", Emoji.BRIEFCASE);

	long refreshInterval;
	String description;
	Emoji emoji;

	ItemType(long refreshInterval, String description, Emoji emoji) {
		this.refreshInterval = refreshInterval;
		this.description = description;
		this.emoji = emoji;
	}

	public long getRefreshInterval() {
		return this.refreshInterval;
	}

	public String getDescription() {
		return description;
	}

	public Emoji getEmoji() {
		return emoji;
	}

	public static ItemType getItemTypeByName(String name) {
		for (ItemType gt : ItemType.values()) {
			if (gt.name().equalsIgnoreCase(name)) {
				return gt;
			}
		}
		return ItemType.CUSTOM;
	}
}
