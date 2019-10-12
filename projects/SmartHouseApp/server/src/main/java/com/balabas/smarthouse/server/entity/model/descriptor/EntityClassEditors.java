package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassEditors {

	DEFAULT("def"),
	STATIC("sta"),
	DEVICE("dev"),
	SERVER("serv"),
	DEVICE_AND_SERVER("devServ");
	
	private String key;
	
	EntityClassEditors(String key) {
		this.key = key;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassEditors from(String key){
		return Stream.of(EntityClassEditors.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(DEFAULT);
	}
}
