package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;
import java.util.Set;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;

@SuppressWarnings("rawtypes")
public interface IEntity extends IEntityAbstract {
	
	Set<IEntityField> getFields();
	
	Set<IEntity> getEntities();
	
	default Optional<IEntity> getEntity(String entityName){
		return getEntities().stream().filter(e -> e.getName().equals(entityName)).findFirst();
	}
	
	default Optional<IEntityField> getEntityField(String entityFieldName){
		return getFields().stream().filter(e -> e.getName().equals(entityFieldName)).findFirst();
	}
	
	IEntityChangeListener getWatcher();
	
	void setWatcher(IEntityChangeListener watcher);
	
	Set<IEntityAlarm> getAlarms();
	
	void setAlarms(Set<IEntityAlarm> alarms);
}
