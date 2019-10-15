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
}
