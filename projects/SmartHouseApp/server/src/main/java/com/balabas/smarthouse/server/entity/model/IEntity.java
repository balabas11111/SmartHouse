package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.model.EntityClass;

@SuppressWarnings("rawtypes")
public interface IEntity extends IItemContainer<IEntityField> {

	int getRemoteId();
	void setRemoteId(int remoteId);
	
	String getDescriptionField();
	void setDescriptionField(String descriptionField);
	
	EntityClass getRenderer();
	void setRenderer(EntityClass entityRenderer);
	
	public Set<String> getSensorItemIds();
	public void setSensorItemIds(Set<String> sensorItemIds);
	
	public Set<String> getSensorItemFields();
	public void setSensorItemFields(Set<String> sensorItemFields);
	
	EntityStatus getStatus();
	void setStatus(EntityStatus status);
	
	String getGroupName();
	void setGroupName(String groupName);
	
	String getDeviceName();
	void setDeviceName(String deviceName);
}
