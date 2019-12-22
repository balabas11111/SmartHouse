package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@MappedSuperclass
@SuppressWarnings("rawtypes")
public abstract class AlarmOfEntityFieldNumber extends AlarmV2 {
	
	protected EntityField entityField;
	
	@Override
	public IItemAbstract getItem() {
		return entityField;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entityField = (EntityField) item;		
	}
	
	@Override
	public boolean accepts(IItemAbstract item) {
		EntityField field = (EntityField) item;
				
		return super.accepts(item) && Number.class.isAssignableFrom(field.getClazz()); 
	}
	
}
