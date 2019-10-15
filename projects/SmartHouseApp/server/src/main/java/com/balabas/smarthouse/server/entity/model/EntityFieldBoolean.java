package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.Getter;
import lombok.Setter;

public class EntityFieldBoolean extends EntityField<Boolean> implements IEntityField<Boolean> {

	@Getter @Setter
	private String action;
	
	@Override
	public void setValueStr(String value) throws BadValueException {
		switch(value){
		case "1": 
			this.value = true; break;
		case "0": 
			this.value = false; break;
		default:
			this.value = Boolean.valueOf(value);
		}
	}

}
