package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassSubstance {

	DEFAULT("default", "Unknown substance"),
	DEVICE("device", "Device root"),
	SETTINGS("settings", "Settings of device"),
	SENSORS("sensors", "Sensors of device"),
	SERVICES("services", "Services of device");
	
	private String key;
	private String description;
	
	EntityClassSubstance(String key, String description) {
		this.key = key;
		this.description = description;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public String getDescription() {
		return this.description;
	}
	
	public static EntityClassSubstance from(String key){
		return Stream.of(EntityClassSubstance.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(DEFAULT);
	}
}
