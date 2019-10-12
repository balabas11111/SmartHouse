package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassPersistor {

	NONE("none"),
	DEVICE("dev"),
	SERVER("serv"),
	DEVICE_AND_SERVER("devServ");
	
	private String key;
	
	EntityClassPersistor(String key) {
		this.key = key;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassPersistor from(String key){
		return Stream.of(EntityClassPersistor.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(NONE);
	}
}
