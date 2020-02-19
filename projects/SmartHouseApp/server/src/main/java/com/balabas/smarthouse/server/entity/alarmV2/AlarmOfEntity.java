package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class AlarmOfEntity extends AlarmV2 {

	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	@Getter @Setter
	private Entity entity;
	
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
