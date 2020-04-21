package com.balabas.smarthouse.server.entity.model.descriptor;

public enum ItemType {
	DEVICE(1000 * 60 * 2, "Устройство", Emoji.PAGER, "device", "dv"),
	SENSORS(1000 * 60 * 1, "Датчики", Emoji.JAPANESE_SYMBOL_FOR_BEGINNER, "sensors", "sr"),
	SETTINGS(1000 * 60 * 5, "Настройки", Emoji.HAMMER_AND_WRENCH, "settings", "st"),
	SERVICES(1000 * 60 * 2, "Сервисы", Emoji.BRIEFCASE, "services", "sr"),
	CUSTOM(-1, "", Emoji.BRIEFCASE, "custom", "cu"),
	GROUP(-1, "Группа", Emoji.BRIEFCASE, "group", "gr"),
	ENTITY(-1, "Датчик", Emoji.BRIEFCASE, "entity", "en"),
	ENTITY_FIELD(-1, "Показатель", Emoji.BRIEFCASE, "EntityField", "ef"),
	ALARM(1000*60, "Тревога", Emoji.BRIEFCASE, "alarm", "al");

	public static final ItemType[] ITEM_TYPES_ORDERED = {DEVICE, GROUP, ENTITY, ENTITY_FIELD};
	
	long refreshInterval;
	String description;
	Emoji emoji;
	String code;
	String shortName;

	ItemType(long refreshInterval, String description, Emoji emoji, String code,String shortName) {
		this.refreshInterval = refreshInterval;
		this.description = description;
		this.emoji = emoji;
		this.code = code;
		this.shortName = shortName;
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
	
	public String getCode() {
		return code;
	}

	public static ItemType getItemTypeByName(String name) {
		if(name == null) {
			return null;
		}
		for (ItemType gt : ItemType.values()) {
			if (gt.name().equalsIgnoreCase(name)) {
				return gt;
			}
		}
		return ItemType.CUSTOM;
	}
	
	public static ItemType getItemTypeByShortName(String name) {
		if(name == null) {
			return null;
		}
		for (ItemType gt : ItemType.values()) {
			if (gt.shortName.equalsIgnoreCase(name)) {
				return gt;
			}
		}
		return null;
	}

	public String getEmojiDescription() {
		return getEmoji().toString() + " " +getDescription();
	}
}
