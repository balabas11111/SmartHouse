package com.balabas.smarthouse.server.entity.model.entityfields;

import java.lang.reflect.ParameterizedType;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.FetchType;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@SuppressWarnings("rawtypes")
@ToString(callSuper = true)
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@javax.persistence.Entity
public abstract class EntityField<T> extends ItemAbstract implements IEntityField<T> {

	@Getter
	@JsonIgnore
	private Class clazz;

	@Getter @Setter
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	private Entity entity;
	
	@Getter
	@Transient
	protected T value;

	@Getter	@Setter
	protected boolean readOnly;

	@Getter	@Setter
	protected EntityFieldClassView viewClass;

	@Getter	@Setter
	@JsonIgnore
	@OneToMany(targetEntity = EntityFieldEnabledValue.class, mappedBy="entityField", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<IEntityFieldEnabledValue> enabledValues;

	@Getter	@Setter
	protected String templateName;

	public EntityField() {
		ParameterizedType type = (ParameterizedType) this.getClass().getGenericSuperclass();
		clazz = (Class) type.getActualTypeArguments()[0];
	}

	@Override
	public void setName(String name) {
		super.setName(name);

		if (name != null && !name.isEmpty() && name.contains(":")) {
			String tmplName = name.substring(name.indexOf(":") + ":".length());
			setTemplateName(tmplName);
		} else {
			setTemplateName(name);
		}
	}

	@Override
	public void setValue(T value) throws BadValueException {
		validateValue(value);

		this.value = value;
	}
	
	@Override
	public void validateValue(T value) throws BadValueException{
		if (value != null && enabledValues != null && !enabledValues.isEmpty()
				&& enabledValues.stream().noneMatch(e -> e.getValue().equals(value))) {
			this.value = null;
			throw new BadValueException();
		}
	}
	
	@Override
	public void setValueWithNoCheck(T value) throws BadValueException {
		this.value = value;
	}

	@Override
	public String getValueStr() {
		if (getValue() == null) {
			return null; 
		}
		return getValue().toString();
	}

	@Override
	public Object getValueObj() {
		return value;
	}

	@Override
	public IEntityFieldEnabledValue getEnabledValueByCurrentValue() {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValue() != null && ev.getValue().equals(getValue()))
				.findFirst().orElse(null);
	}
	
}
