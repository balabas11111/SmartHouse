package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
@SuppressWarnings("rawtypes")
public class EntityFieldIncorrectValue implements IEntityFieldIncorrectValue {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	protected Long id = 0L;
	
	@JsonIgnore
	@Getter @Setter
	@ManyToOne(targetEntity = EntityField.class, fetch = FetchType.LAZY)
    @JoinColumn( name="entity_field_id", nullable=false)
	private IEntityField entityField;
	
	@Getter @Setter
	private String value;
	
	@Getter @Setter
	private Date date;
}
