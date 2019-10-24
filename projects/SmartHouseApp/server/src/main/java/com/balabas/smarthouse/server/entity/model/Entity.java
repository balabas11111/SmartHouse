package com.balabas.smarthouse.server.entity.model;

import java.util.LinkedHashSet;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.ElementCollection;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@SuppressWarnings("rawtypes")
@Data
@EqualsAndHashCode(callSuper = true, exclude = "entityFields")
@Table(name="entities")
@javax.persistence.Entity
public class Entity extends ItemAbstract implements IEntity {

	private int remoteId;
	private String descriptionField;
	private EntityClass renderer;

	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="group_id", nullable=false)
	private Group group;
	
	@ElementCollection(fetch = FetchType.EAGER)
	private Set<String> grouppedFieldsIds;
	@ElementCollection(fetch = FetchType.EAGER)
	private Set<String> grouppedFieldsNames;
	/*@Transient
	private Set<IEntityField> generatedFields;
*/
	@JsonIgnore
	@OneToMany(targetEntity = EntityField.class, mappedBy = "entity", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<IEntityField> entityFields;
	
	@Transient
	private EntityStatus status;
	
	@Enumerated(EnumType.STRING)
	private State state;

	@Override
	public IEntityField getEntityField(String entityFieldName) {
		if(getEntityFields()==null) {
			return null;
		}
		return getEntityFields().stream().filter( ef -> ef.getName().equalsIgnoreCase(entityFieldName)).findFirst().orElse(null);
	}

	@Override
	public void addEntityField(IEntityField entityField) {
		this.entityFields = addEntityFieldWithCheck(this.entityFields, entityField);
	}
/*
	@Override
	public void addGeneratedField(IEntityField entityField) {
		this.generatedFields = addEntityFieldWithCheck(this.generatedFields, entityField);
	}
*/
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
