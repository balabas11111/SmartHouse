package com.balabas.smarthouse.server.entity.model;

import java.util.LinkedHashSet;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@SuppressWarnings("rawtypes")
@Data
@EqualsAndHashCode(callSuper = true)

public class Entity extends ItemAbstract implements IEntity {

	private int remoteId;
	private String descriptionField;
	private EntityClass renderer;

	private String groupName;
	private String deviceName;
	
	private Set<String> grouppedFieldsIds;
	private Set<String> grouppedFieldsNames;
	private Set<IEntityField> generatedFields;

	@JsonIgnore
	protected Set<IEntityField> children;
	
	private EntityStatus status;
	private State state;

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
		if (fields == null) {
			fields = new LinkedHashSet<>();
			fields.add(entityField);
		} else {
			if (!fields.contains(entityField)
					&& fields.stream().noneMatch(ef -> ef.getName().equals(entityField.getName()))) {

				fields.add(entityField);
			}
		}
		return fields;
	}

}
