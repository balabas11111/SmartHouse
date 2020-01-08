package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.view.Action;

@SuppressWarnings("rawtypes")
public interface IEntityService {

	List<Action> getActionsForEntity(String actionName, IEntity entity);
	
	List<IEntityFieldEnabledValue> getEnabledEntityFieldWithCommandsForEntity(IEntity entity);

	void deleteEntitiesForDevice(Long deviceId);

	List<IEntity> loadVirtualized();

	IEntity loadEntityById(Long id);
}
