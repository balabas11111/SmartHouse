package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassType {

	BOOLEAN(Boolean.class, "bool"),
	INTEGER(Integer.class, "int"),
	LONG(Long.class, "long"),
	FLOAT(Float.class, "float"),
	STRING(String.class, "char*"),
	OBJECT(Object.class, "obj");
		
	private Class<?> clazz;
	private String key;
	
	EntityClassType(Class<?> clazz, String key) {
		this.clazz = clazz;
		this.key = key;
	}
	
	public Class<?> getClazz() {
		return this.clazz;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassType from(String key){
		return Stream.of(EntityClassType.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(OBJECT);
	}
}
