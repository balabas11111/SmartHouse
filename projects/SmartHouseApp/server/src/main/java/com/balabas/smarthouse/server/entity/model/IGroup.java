package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IGroup extends IItemTypeable, IItemAbstract {

	Device getDevice();
	void setDevice(Device device);
	
	public Set<Entity> getEntities();
	void setEntities(Set<Entity> entities);
	
	Entity getEntity(String entityName);
	void setItemType(ItemType sensors);
	ItemType getType();
	
}
