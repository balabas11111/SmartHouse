package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public class Group extends ItemContainer<IEntity> implements IGroup {

	private String deviceName;

	@Override
	public Set<IEntity> getEntities() {
		return getChildren();
	}
	
	@Override
	public String getDeviceName() {
		return this.deviceName;
	}

	@Override
	public void setDeviceName(String deviceName) {
		this.deviceName = deviceName;
	}
}
