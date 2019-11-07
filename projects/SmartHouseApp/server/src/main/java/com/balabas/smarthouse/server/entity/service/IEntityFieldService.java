package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.Action;

@SuppressWarnings("rawtypes")
public interface IEntityFieldService {

	List<Action> getActionsForEntityField(String actionName, IEntityField entityField);
	
	List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField);

	boolean isButton(IEntityField entityField);

	
}
