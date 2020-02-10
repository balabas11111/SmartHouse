package com.balabas.smarthouse.server.entity.model.descriptor;

public enum AbstractItemType {
	UNKNOWN("Неведомая фигня", Emoji.PAGER, "unknown"),
	ALARM("Тревога", Emoji.PAGER, "alarm"),
	DEVICE("Устройство", Emoji.PAGER, "device"),
	GROUP("Группа", Emoji.BRIEFCASE, "group"),
	ENTITY("Датчик", Emoji.JAPANESE_SYMBOL_FOR_BEGINNER, "entity"),
	ENTITY_FIELD("Показатель", Emoji.BAR_CHART, "entity_field"),;

	String description;
	Emoji emoji;
	String name;

	AbstractItemType(String description, Emoji emoji, String name) {
		this.description = description;
		this.emoji = emoji;
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public Emoji getEmoji() {
		return emoji;
	}
	
	public String getName() {
		return name;
	}

	public static AbstractItemType getItemTypeByName(String name) {
		for (AbstractItemType gt : AbstractItemType.values()) {
			if (gt.name().equalsIgnoreCase(name)) {
				return gt;
			}
		}
		return AbstractItemType.UNKNOWN;
	}
}
