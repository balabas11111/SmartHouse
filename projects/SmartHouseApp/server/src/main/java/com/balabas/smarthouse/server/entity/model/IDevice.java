package com.balabas.smarthouse.server.entity.model;

public interface IDevice extends IItemContainer<IGroup> {

	String getFirmware();
	void setFirmware(String firmware);
}
