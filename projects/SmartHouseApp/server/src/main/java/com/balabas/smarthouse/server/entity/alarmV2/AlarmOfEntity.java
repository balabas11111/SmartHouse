package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@MappedSuperclass
public abstract class AlarmOfEntity extends AlarmV2 {

	protected Entity entity;
	
	@Override
	public IItemAbstract getItem() {
		return entity;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entity = (Entity) item;		
	}
	
}
