package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IItemContainer<T extends IItemAbstract> extends IItemAbstract {

	Set<T> getChildren();
	void setChildren(Set<T> children);
	
	String getDeviceName();
	void setDeviceName(String deviceName);
	
	default  T getChild(String childName) {
		return getChildren().stream().filter(c -> c.getName().equals(childName)).findFirst().orElse(null);
	}

	default boolean hasChild(String childName) {
		return getChild(childName) != null;
	}
}
