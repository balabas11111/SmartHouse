package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

public interface IDevice extends IStateable, IUpdateable, IItemAbstract {

	String getFirmware();
	void setFirmware(String firmware);

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
	Entity getEntityByRemoteId(int remoteId);
	
	Device update(Device device);
	
}
