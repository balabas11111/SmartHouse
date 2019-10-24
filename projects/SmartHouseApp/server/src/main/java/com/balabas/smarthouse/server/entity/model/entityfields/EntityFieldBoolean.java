package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;

@Entity
public class EntityFieldBoolean extends EntityField<Boolean> implements IEntityField<Boolean> {

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

}
