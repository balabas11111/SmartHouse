package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@MappedSuperclass
@SuppressWarnings("rawtypes")
public abstract class AlarmOfEntityField extends AlarmV2 {
	
	protected EntityField entityField;
	
	@Override
	public IItemAbstract getItem() {
		return entityField;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entityField = (EntityField) item;		
	}
	
}
