package com.balabas.smarthouse.server.entity.model;

import java.util.LinkedHashSet;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@SuppressWarnings("rawtypes")
@Data
@EqualsAndHashCode(callSuper = true)
public class Entity extends ItemContainer<IEntityField> implements IEntity {

	private int remoteId;
	private String descriptionField;
	private EntityClass renderer;
	
	private Set<String> grouppedFieldsIds;
	private Set<String> grouppedFieldsNames;
	private Set<IEntityField> generatedFields;
	
	private EntityStatus status;
	private State state;
	
	@JsonIgnore
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

	@Override
	public IEntityField getField(String fieldName) {
		return getChild(fieldName);
	}

	@Override
	public Set<IEntityField> getEntityFields() {
		return getChildren();
	}

	@Override
	public void setEntityFields(Set<IEntityField> fields) {
		setChildren(fields);
	}
	
	@Override
	public void addEntityField(IEntityField entityField) {
		this.children = addEntityFieldWithCheck(this.children, entityField);
	}
	
	@Override
	public void addGeneratedField(IEntityField entityField) {
		this.generatedFields = addEntityFieldWithCheck(this.generatedFields, entityField);
	}
	
	private Set<IEntityField> addEntityFieldWithCheck(Set<IEntityField> fields, IEntityField entityField) {
		if(fields == null) {
			fields = new LinkedHashSet<>();
			fields.add(entityField);
		} else {
			if(!fields.contains(entityField)
					&& fields.stream().noneMatch( ef -> ef.getName().equals(entityField.getName()))) {
				
				fields.add(entityField);
			}
		}
		return fields;
	}
}
