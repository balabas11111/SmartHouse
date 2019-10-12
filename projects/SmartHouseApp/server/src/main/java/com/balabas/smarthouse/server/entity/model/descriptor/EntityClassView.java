package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassView {

	DEFAULT("default"),
	NONE("none"),
	LIST("list"),
	TABLE("table"),
	MAP("map"),
	LABEL("text"),
	INPUT("input"),
	SELECT("select"),
	BUTTON("button");
	
	private String key;
	
	EntityClassView(String key) {
		this.key = key;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassView from(String key){
		return Stream.of(EntityClassView.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(DEFAULT);
	}
}
