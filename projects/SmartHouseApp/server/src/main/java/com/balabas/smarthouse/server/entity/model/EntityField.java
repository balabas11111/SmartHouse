package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import org.springframework.core.ResolvableType;

import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
public class EntityField<T> extends EntityAbstractDescripted implements IEntityField<T> {

	@Getter
	@JsonIgnore
	private Class clazz;
	
	@Getter
	private T value;
	
	@Getter
	@JsonIgnore
	private String valueStr;
	
	@Getter @Setter
	private Set<IEntityFieldAlarm> alarms;
	
	public EntityField() {
		this.clazz = ResolvableType.forClass(EntityField.class).getClass();
	}
	
	@Override
	public void setValue(T value) throws BadValueException {
		boolean changed = (value!=null && value.equals(this.value)) 
							|| (this.value!=null && value.equals(value));
		
		this.valueStr = fromValueToString(value);
		this.value = value;
		
		watch();
	}

	@Override
	public void setValueStr(String valueStr) throws BadValueException {
		boolean changed = (valueStr!=null && valueStr.equals(this.valueStr)) 
				|| (this.valueStr!=null && value.equals(valueStr));
		
		this.valueStr = valueStr; 
		this.value = fromStringToValue(valueStr);
		
		watch();
	}
	
	protected T fromStringToValue(String value) throws BadValueException {
		return EntityFieldUtil.fromStringToValue(value, getClazz());
	}
	
	protected String fromValueToString(T value) throws BadValueException {
		return EntityFieldUtil.fromValueToString(value, getClazz());
	}
	
}
