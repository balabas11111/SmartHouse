package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import org.springframework.core.ResolvableType;

import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
public abstract class EntityField<T> extends ItemAbstract implements IEntityField<T> {

	@Getter
	@JsonIgnore
	private Class clazz;

	@Getter
	protected T value;
	
	@Getter @Setter
	protected boolean readOnly;
	
	@Getter @Setter
	protected EntityFieldClassView viewClass;

	protected Set<EntityField<T>> enabledValues;

	public EntityField() {
		this.clazz = ResolvableType.forClass(EntityField.class).getClass();
	}

	@Override
	public void setValue(T value) throws BadValueException {
		if (enabledValues != null && !enabledValues.isEmpty() && enabledValues.stream().noneMatch( e -> e.getValue().equals(value))) {
			throw new BadValueException();
		}
		this.value = value;
	}

}
