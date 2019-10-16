package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IGroup extends IItemTypeable, IUpdateable, IItemContainer<IEntity> {

	public Set<IEntity> getEntities();
	void setEntities(Set<IEntity> entities);
	
	default IEntity getEntity(String entityName) {
		return getChild(entityName);
	}
	
}
