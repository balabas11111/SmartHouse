package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.UpdateTimer;

public interface IUpdateable {

	UpdateTimer getUpdateTimer();

	void setUpdateTimer(UpdateTimer updateTimer);
}
