package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IGroup extends IItemContainer<IEntity> {

	public Set<IEntity> getEntities();

}
