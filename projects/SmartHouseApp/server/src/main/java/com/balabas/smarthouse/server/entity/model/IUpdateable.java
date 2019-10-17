package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;

public interface IUpdateable {

	ActionTimer getTimer();

	void setTimer(ActionTimer updateTimer);
}
