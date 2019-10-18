package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;

@SuppressWarnings("rawtypes")
public interface IEntity extends IStateable, IItemContainer<IEntityField> {

	IEntityField getField(String fieldName);
	Set<IEntityField> getEntityFields();
	
	int getRemoteId();
	void setRemoteId(int remoteId);
	
	String getDescriptionField();
	void setDescriptionField(String descriptionField);
	
	EntityClass getRenderer();
	void setRenderer(EntityClass entityRenderer);
	
	Set<String> getGrouppedFieldsIds();
	void setGrouppedFieldsIds(Set<String> grouppedFieldsIds);
	
	Set<String> getGrouppedFieldsNames();
	void setGrouppedFieldsNames(Set<String> grouppedFieldsNames);
	
	EntityStatus getStatus();
	void setStatus(EntityStatus status);
	
	String getGroupName();
	void setGroupName(String groupName);
	
	String getDeviceName();
	void setDeviceName(String deviceName);
	
	default IEntityField getEntityField(String entityFieldName) {
		return getChild(entityFieldName);
	}
}
