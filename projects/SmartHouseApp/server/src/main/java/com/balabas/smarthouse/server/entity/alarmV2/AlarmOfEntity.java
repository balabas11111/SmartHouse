package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@javax.persistence.Entity
public class AlarmOfEntity extends AlarmV2 {

	protected Entity entity;
	
	@Override
	public Class<?> getTargetItemClass() {
		return Entity.class;
	}
	
	@Override
	public IItemAbstract getItem() {
		return entity;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entity = (Entity) item;		
	}
	
}
