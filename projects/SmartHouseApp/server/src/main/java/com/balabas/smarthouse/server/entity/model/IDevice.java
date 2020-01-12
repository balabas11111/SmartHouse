package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IDevice extends IStateable, IItemAbstract {

	String getFirmware();
	void setFirmware(String firmware);

	String getStateEmoji();
	String getStateDescription();
	
	boolean isInBadState();
	boolean isRegistered();
	boolean isInitialized();
	void setInitialized(boolean initialized);

	Set<Group> getGroups();
	void setGroups(Set<Group> groups);
	Set<Entity> getEntities();
	
	String getIp();
	String getDataUrl();
	
	Group getGroup(String groupName);
	
	Entity getEntity(String entityName);
	Entity getEntity(Long entityId);
	//Entity getEntityByRemoteId(int remoteId);
	IEntityField getEntityField(Long entityId, Long entityFieldId);
	boolean isInOkState();
	Set<IEntityField> getEntityFields();
	
}
