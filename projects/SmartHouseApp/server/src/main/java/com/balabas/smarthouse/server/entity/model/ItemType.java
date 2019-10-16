package com.balabas.smarthouse.server.entity.model;

public enum ItemType {
	DEVICE(1000 * 60 * 2, "Устройство"),
	SENSORS(1000 * 60 * 1, "Датчики"),
	SETTINGS(1000 * 60 * 5, "Настройки"),
	SERVICES(1000 * 60 * 2, "Сервисы"),
	CUSTOM(-1, "");

	long refreshInterval;
	String description;

	ItemType(long refreshInterval, String description) {
		this.refreshInterval = refreshInterval;
		this.description = description;
	}

	long getRefreshInterval() {
		return this.refreshInterval;
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
