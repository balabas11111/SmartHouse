package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Optional;
import java.util.Set;

import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
@javax.persistence.Entity
public class EntityFieldBoolean extends EntityField<Boolean> implements IEntityField<Boolean> {

	@ManyToOne
	@JoinColumn(name="target_entity_field_id", nullable=true)
	@Setter
	private EntityFieldBoolean targetEntityField;
	
	@Transient
	@JsonAlias("value")
	@Getter @Setter
	private Boolean valueB;
	
	@Override
	public Class<Boolean> getClazz() {
		return Boolean.class;
	}
	
	@Override
	public Entity getTargetEntity() {
		return getRealField().getEntity();
	}
	
	public Long getTargetEntityFieldId() {
		if(targetEntityField!=null) {
			return targetEntityField.getId();
		}
		return null;
	}
	
	public EntityFieldBoolean getTargetEntityField() {
		return targetEntityField;
	}
	
	@Override
	public Set<IEntityFieldEnabledValue> getEnabledValues() {
		if(targetEntityField!=null) {
			return targetEntityField.getEnabledValues();
		}
		return this.enabledValues;
	}
	
	@JsonIgnore
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueByCurrentValue() {
		IEntityFieldEnabledValue target = null;
		if(this.targetEntityField!=null) {
			target = this.targetEntityField.getEntityFieldEnabledValueByCurrentValue();
		} else {
			target = super.getEntityFieldEnabledValueByCurrentValue();
		}
		return target;
	}
	
	@Override
	protected Boolean fromString(String value) {
		Boolean val = null;
		
		if(value!=null){
			switch(value){
			case "1": 
				val = true; break;
			case "0": 
				val = false; break;
			default:
				val = Boolean.valueOf(value);
			}
		}
		
		return val;
	}

	@Override
	public Boolean getValue() {
		return getRealField().getValueB();
	}

	@Override
	public boolean setValue(Boolean value) {
		boolean equals = equalsAsValue(getValue(), value);

		if(!equals) {
			getRealField().setValueB(value);
		}
		
		return !equals;
	}

	@Override
	public String getValueMeasureStr() {
		return getValue().booleanValue()? "Включено": "Отключено";
	}
	
	@Override
	public Boolean getValueAsBoolean() {
		return this.getValue();
	}

	private EntityFieldBoolean getRealField() {
		if(!isVirtualized()) {
			return this;
		}
		return Optional.ofNullable(this.targetEntityField).orElse(this);
	}
}
