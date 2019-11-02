package com.balabas.smarthouse.server.entity.model;

import java.util.Date;

import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@javax.persistence.Entity
@SuppressWarnings("rawtypes")
public abstract class EntityFieldValue implements IEntityFieldValue {
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	protected Long id = 0L;
	
	@JsonIgnore
	@Getter @Setter
	@ManyToOne(targetEntity = EntityField.class, fetch = FetchType.LAZY)
    @JoinColumn( name="entity_field_id", nullable=false)
	protected IEntityField entityField;
	
	@Getter
	protected Date date;
	
}
