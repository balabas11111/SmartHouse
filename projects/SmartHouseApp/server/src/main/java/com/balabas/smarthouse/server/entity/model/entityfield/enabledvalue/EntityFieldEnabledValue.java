package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.EntityField;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@javax.persistence.Entity
public abstract class EntityFieldEnabledValue<T> implements IEntityFieldEnabledValue<T> {

	@Id
	@GeneratedValue(strategy = GenerationType.TABLE)
	Long id;
	
	@Getter @Setter
	@ManyToOne(fetch = FetchType.EAGER)
	@JoinColumn(name="entity_field_id", nullable=false)
	protected EntityField entityField;
	
	@Getter @Setter
	private String description;
	
	@Getter @Setter
	private String actionDescription;
	
	@Getter @Setter
	private Emoji emoji;
	
	@Getter @Setter
	private EntityFieldClassView viewClass;
	
}
