package com.balabas.smarthouse.server.entity.model.enabledvalue;

import javax.persistence.Column;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class EntityFieldEnabledValueBoolean extends EntityFieldEnabledValue<Boolean> implements IEntityFieldEnabledValue<Boolean> {

	@Getter @Setter
	@Column(name = "valueBool")
	protected Boolean value;
	
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

	@Override
	public String getValueStr() {
		return Boolean.toString(this.value);
	}
}
