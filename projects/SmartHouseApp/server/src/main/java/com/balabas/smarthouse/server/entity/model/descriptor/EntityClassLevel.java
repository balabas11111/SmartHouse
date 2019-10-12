package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassLevel {

	DEFAULT("default"),
	DEVICE("device"),
	GROUP("group"),
	ITEM("item"),
	FIELD("field");
	
	private String key;
	
	EntityClassLevel(String key) {
		this.key = key;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassLevel from(String key){
		return Stream.of(EntityClassLevel.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(DEFAULT);
	}
}
