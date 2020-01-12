package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldBoolean extends EntityField<Boolean> implements IEntityField<Boolean> {

	@Override
	public Class<Boolean> getClazz() {
		return Boolean.class;
	}
	
	@Transient
	@JsonAlias("value")
	private Boolean valueB;
	
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
		return this.valueB;
	}

	@Override
	public boolean setValue(Boolean value) {
		boolean equals = equalsAsValue(this.valueB, value);

		if(!equals) {
			this.valueB = value;
		}
		
		return !equals;
	}

	@Override
	public String getValueMeasureStr() {
		return getValue().booleanValue()? "Включено": "Отключено";
	}

}
