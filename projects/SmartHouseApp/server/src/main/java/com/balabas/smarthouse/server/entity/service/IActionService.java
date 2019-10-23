package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.view.Action;

public interface IActionService {

	boolean executeDeviceAction(Action action) throws ResourceNotFoundException;

	Action getDeviceGroupEntityNameActionByIdAction(Action action);
}
