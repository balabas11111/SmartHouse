package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Optional;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;

import org.thymeleaf.util.StringUtils;

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

	@JsonIgnore
	@Getter @Setter
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	private Entity entity;

	@Getter	@Setter
	protected boolean readOnly;
	
	@Getter	@Setter
	protected boolean active;
	
	@Getter	@Setter
	protected boolean calculated;
	
	@Getter	@Setter
	protected String descriptionField;

	@Setter
	protected String measure;
	
	@Getter	@Setter
	@Enumerated(EnumType.STRING)
	protected EntityFieldClassView viewClass;

	@Getter	@Setter
	@OneToMany(targetEntity = EntityFieldEnabledValue.class, mappedBy="entityField", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<IEntityFieldEnabledValue> enabledValues;

	@Getter	@Setter
	protected String templateName;

	public abstract Class getClazz();

	public abstract void setValue(T value);
	
	protected abstract T fromString(String value);

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
	public void setValueWithCheck(T value) throws BadValueException {
		validateValue(value);

		setValue(value);
	}
	
	@Override
	public void validateValue(T value) throws IllegalArgumentException{
		if (value != null && enabledValues != null && !enabledValues.isEmpty()
				&& enabledValues.stream().noneMatch(e -> e.getValue().equals(value))) {
			setValue(null);
			throw new IllegalArgumentException();
		}
	}
	
	@Override
	public void setValueStr(String value) throws BadValueException {
		setValueWithCheck(fromString(value));
	}
	
	@Override
	public void setValueWithNoCheck(T value) {
		setValue(value);
	}
	
	@Override
	public void setValueWithNoCheckStr(String value) {
		setValueWithNoCheck(fromString(value));
	}

	@JsonIgnore
	@Override
	public String getValueStr() {
		if (getValue() == null) {
			return null; 
		}
		String result = getValue().toString();
		return result;
	}
	
	@JsonIgnore
	@Override
	public String getMeasure() {
		return measure==null?"":measure;
	}

	@JsonIgnore
	@Override
	public Object getValueObj() {
		return getValue();
	}
	
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValue(T value) {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValue() != null && ev.getValue().equals(value))
				.findFirst().orElse(null);
	}
	
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueByValueStr(String value) {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValueStr() != null && ev.getValueStr().equals(value))
				.findFirst().orElse(null);
	}

	@JsonIgnore
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueByCurrentValue() {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValue() != null && ev.getValue().equals(getValue()))
				.findFirst().orElse(null);
	}
	
	@JsonIgnore
	@Override
	public boolean isButton() {
		return Boolean.class.equals(getClazz())
				&& getViewClass().isButton();
	}
	
	@Override
	public String getNameDescriptionByDescriptionField() {
		return getName() +" (" + getDescriptionByDescriptionField() + ")";
	}
	
	@Override
	public String getDescriptionByDescriptionField() {
		if(StringUtils.isEmpty(descriptionField)) {
			return Optional.ofNullable(description).orElse("");
		}
		
		Optional<IEntityField> descrField = Optional.ofNullable(entity.getEntityField(descriptionField));
		
		if(descrField.isPresent()) {
			return Optional.ofNullable(descrField.get().getValueStr()).orElse(description);
		}
		
		return description;
	}
	
}
