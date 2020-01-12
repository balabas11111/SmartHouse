package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@EqualsAndHashCode(exclude = {"entityField"})
@javax.persistence.Entity
public abstract class EntityFieldEnabledValue<T> implements IEntityFieldEnabledValue<T> {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Setter
	@Getter
	Long id;
	
	@JsonIgnore
	@Getter @Setter
	@ManyToOne(fetch = FetchType.EAGER)
	@JoinColumn(name="entity_field_id", nullable=false)
	protected EntityField entityField;
	
	@Getter @Setter
	private String description;
	
	@Getter @Setter
	private String actionDescription;
	
	@Getter @Setter
	@Enumerated(EnumType.STRING)
	private Emoji emoji;
	
	@Getter @Setter
	@Enumerated(EnumType.STRING)
	private EntityFieldClassView viewClass;
	
	@Override
	public String buildDataForCallBack() {
		return new JSONObject()
				.put(entityField.getName(), getValueStr()).toString();
	}
	
}
