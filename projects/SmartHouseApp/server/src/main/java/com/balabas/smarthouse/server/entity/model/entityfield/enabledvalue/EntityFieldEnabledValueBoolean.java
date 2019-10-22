package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

@javax.persistence.Entity
public class EntityFieldEnabledValueBoolean extends EntityFieldEnabledValue<Boolean> implements IEntityFieldEnabledValue<Boolean> {

	@Override
	public void setValueStr(String value) {
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
