package com.balabas.smarthouse.server.entity.model;

import java.util.LinkedHashSet;
import java.util.Optional;
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

import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@SuppressWarnings("rawtypes")
@Data
@EqualsAndHashCode(callSuper = true, exclude = {"entityFields", "grouppedFieldsIds", "grouppedFieldsNames"})
@Table(name="entities")
@javax.persistence.Entity
public class Entity extends ItemAbstract implements IEntity {

	private boolean hasMq;
	private String descriptionField;

	@JsonIgnore
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="group_id", nullable=false)
	private Group group;
	
	@ElementCollection(fetch = FetchType.EAGER)
	private Set<String> grouppedFieldsIds;
	@ElementCollection(fetch = FetchType.EAGER)
	private Set<String> grouppedFieldsNames;

	@OneToMany(targetEntity = EntityField.class, mappedBy = "entity", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<IEntityField> entityFields;
	
	@Transient
	private EntityStatus status;
	
	@Enumerated(EnumType.STRING)
	private State state;

	@JsonIgnore
	@Override
	public Device getDevice() {
		if(getGroup() == null) {
			return null;
		}
		return getGroup().getDevice(); 
	}
	
	@Override
	public ItemType getItemType() {
		if(getGroup() == null) {
			return null;
		}
		return getGroup().getType();
	}
	
	@Override
	public IEntityField getEntityField(Long entityFieldId) {
		if(getEntityFields()==null) {
			return null;
		}
		return getEntityFields().stream().filter( ef -> ef.getId().equals(entityFieldId)).findFirst().orElse(null);
	}
	
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
	
	@Override
	public String getDescriptionByDescriptionField() {
		if(StringUtils.isEmpty(descriptionField)) {
			return Optional.ofNullable(description).orElse("");
		}
		
		Optional<IEntityField> descrField = Optional.ofNullable(getEntityField(descriptionField));
		
		if(descrField.isPresent()) {
			return Optional.ofNullable(descrField.get().getValueStr()).orElse(description);
		}
		
		return description;
	}
	
	@Override
	public String getEntityKey() {
		return getDevice().getName() + getName();
	}
	
	@Override
	public void setParent(IItemAbstract parent) {
		if(parent!=null && !Group.class.isAssignableFrom(parent.getClass())) {
			throw new IllegalArgumentException("IEntity expected");
		}
		setGroup((Group) parent);
	}

}
