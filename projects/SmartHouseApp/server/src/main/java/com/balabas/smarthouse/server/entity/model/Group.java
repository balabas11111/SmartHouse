package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public class Group extends ItemContainer<IEntity> implements IGroup {

	@Override
	public Set<IEntity> getEntities() {
		return getChildren();
	}
}
