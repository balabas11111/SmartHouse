package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.model.UpdateTimer;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class Group extends ItemContainer<IEntity> implements IGroup {

	private ItemType type;

	@JsonIgnore
	private UpdateTimer updateTimer;
	
	@Override
	public Set<IEntity> getEntities() {
		return getChildren();
	}
	
	@Override
	public void setEntities(Set<IEntity> entities) {
		setChildren(entities);
	}
	
	@Override
	public String getDeviceName() {
		return this.parentName;
	}

	@Override
	public void setDeviceName(String deviceName) {
		this.parentName = deviceName;
	}
	
}
