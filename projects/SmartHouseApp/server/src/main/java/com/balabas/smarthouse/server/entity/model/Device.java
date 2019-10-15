package com.balabas.smarthouse.server.entity.model;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class Device extends ItemContainer<IGroup> implements IDevice {

	private String firmware;

	@Override
	public String getDeviceName() {
		return getName();
	}

	@Override
	public void setDeviceName(String deviceName) {
		setName(deviceName);
	}
}
