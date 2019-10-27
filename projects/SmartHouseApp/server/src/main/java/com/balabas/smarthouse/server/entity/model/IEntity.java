package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntity extends IStateable, IItemAbstract {

	Set<IEntityField> getEntityFields();
	void setEntityFields(Set<IEntityField> fields);
	
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
	
	Group getGroup();
	void setGroup(Group group);
	
	Device getDevice();
	
	IEntityField getEntityField(String entityFieldName);
	
	void addEntityField(IEntityField entityField);
	
	/*
	  void addGeneratedField(IEntityField entityField);
	  Set<IEntityField> getGeneratedFields();
	 */
}
