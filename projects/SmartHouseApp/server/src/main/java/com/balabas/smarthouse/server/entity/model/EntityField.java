package com.balabas.smarthouse.server.entity.model;

import java.lang.reflect.ParameterizedType;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@SuppressWarnings("rawtypes")
@ToString(callSuper = true)
public abstract class EntityField<T> extends ItemAbstract implements IEntityField<T> {

	@Getter
	@JsonIgnore
	private Class clazz;

	@Getter
	protected T value;
	
	@Getter @Setter
	private String action;
	
	@Getter @Setter
	protected boolean readOnly;
	
	@Getter @Setter
	protected EntityFieldClassView viewClass;

	@Getter @Setter
	protected Set<IEntityField<T>> enabledValues;

	public EntityField() {
		ParameterizedType type = (ParameterizedType)this.getClass().getGenericSuperclass();
		clazz = (Class)type.getActualTypeArguments()[0];
	}

	@Override
	public void setValue(T value) throws BadValueException {
	
		if (value!=null && enabledValues != null && !enabledValues.isEmpty() && enabledValues.stream().noneMatch( e -> e.getValue().equals(value))) {
			this.value = null;
			throw new BadValueException();
		}
		
		this.value = value;
	}
	
	@Override
	public void setValueWithNoCheck(T value) throws BadValueException {
		this.value = value;
	}
	
	@Override
	public String getValueStr() {
		return getValue().toString();
	}
	
	@Override
	public Object getValueObj(){
		return value;
	}

}
