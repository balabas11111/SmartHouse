package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntity extends IStateable, IItemAbstract {

	Set<IEntityField> getEntityFields();
	void setEntityFields(Set<IEntityField> fields);

	String getDescriptionField();
	void setDescriptionField(String descriptionField);

	Set<String> getGrouppedFieldsIds();
	void setGrouppedFieldsIds(Set<String> grouppedFieldsIds);
	
	Set<String> getGrouppedFieldsNames();
	void setGrouppedFieldsNames(Set<String> grouppedFieldsNames);
	
	EntityStatus getStatus();
	void setStatus(EntityStatus status);
	
	Group getGroup();
	void setGroup(Group group);
	
	Device getDevice();
	
	ItemType getItemType();
	
	IEntityField getEntityField(String entityFieldName);
	
	void addEntityField(IEntityField entityField);
	IEntityField getEntityField(Long entityFieldId);
	String getDescriptionByDescriptionField();
	
	boolean isHasMq();
	void setHasMq(boolean hasMq);
	
	String getEntityKey();
}
