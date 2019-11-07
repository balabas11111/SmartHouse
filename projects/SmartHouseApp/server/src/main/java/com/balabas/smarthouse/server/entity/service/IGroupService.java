package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.view.Action;

public interface IGroupService {

	List<Action> getActionsForGroup(String actionName, IGroup group);
	
}
