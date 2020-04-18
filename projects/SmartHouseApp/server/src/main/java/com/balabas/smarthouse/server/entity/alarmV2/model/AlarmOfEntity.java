package com.balabas.smarthouse.server.entity.alarmV2.model;

import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class AlarmOfEntity extends AlarmV2 {

	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	@Getter @Setter
	private Entity entity;
	
	@Getter
	@Transient
	private final Class<?> targetItemClass = Entity.class;
	
	@Getter
	@Transient
	private final ItemType itemType = ItemType.ENTITY;
	
	@Override
	public IItemAbstract getItem() {
		return entity;
	}

	@Override
	public void setItem(IItemAbstract item) {
		this.entity = (Entity) item;		
	}
	
}
