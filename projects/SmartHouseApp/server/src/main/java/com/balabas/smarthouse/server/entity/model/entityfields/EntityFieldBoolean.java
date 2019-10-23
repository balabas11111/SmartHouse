package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

@Entity
public class EntityFieldBoolean extends EntityField<Boolean> implements IEntityField<Boolean> {

	@Override
	public void setValueStr(String value) throws BadValueException {
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
		
		setValue(val);
	}

}