package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.model.EntityClass;
import lombok.Data;
import lombok.EqualsAndHashCode;

@SuppressWarnings("rawtypes")
@Data
@EqualsAndHashCode(callSuper = true)
public class Entity extends ItemContainer<IEntityField> implements IEntity {

	private int remoteId;
	private String descriptionField;
	private EntityClass renderer;
	
	private Set<String> sensorItemIds;
	private Set<String> sensorItemFields;
	
	private EntityStatus status;
	private String devName;

	@Override
	public String getGroupName() {
		return this.parentName;
	}

	@Override
	public void setGroupName(String groupName) {
		this.parentName = groupName;
	}

	@Override
	public String getDeviceName() {
		return this.devName;
	}

	@Override
	public void setDeviceName(String deviceName) {
		this.devName = deviceName;
	}
}
