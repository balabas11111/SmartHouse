package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.List;

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
	
	IDevice createDevice(String name, String description, String firmware);
	
	IDevice getDeviceById(Long id);
	
	List<IDevice> getDevices();
	
	List<IGroup> getGroups();
	
	List<IEntity> getEntities();
	
	List<IEntityField> getEntityFields();
	
	void save(IDevice device);
	
	void save(IGroup group);
	
	void save(IEntity entity);
	
	void save(IEntityField entityField);
}
