package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.alarm.IEntityAlarm;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
public class Entity extends EntityAbstractDescripted implements IEntity {
	
	@Getter @Setter
	private Set<IEntityField> fields;
	
	@Getter @Setter
	private Set<IEntity> entities;
	
	@JsonIgnore
	@Getter @Setter
	private Set<IEntityAlarm> alarms;
	
}
