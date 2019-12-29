package com.balabas.smarthouse.server.entity.model.collectors;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IVirtualEntityService {
	
	IEntityField createEntityFieldFloat(String name, String description);
	
	IEntityField getEntityFieldById(Long id);
	
	IEntity createEntity(String name, String description);
	
	IEntity getEntityById(Long id);
	
	IGroup createGroup(String name, String description);
	
	IGroup getGroupById(Long id);
	
	IDevice createDevice(String name, String description);
	
	IDevice getDeviceById(Long id);
}
