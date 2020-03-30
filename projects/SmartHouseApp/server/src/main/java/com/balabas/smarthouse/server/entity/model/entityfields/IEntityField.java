package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IEntityField<T> extends IItemAbstract {

	Class getClazz();
	
	T getValue();
	void setValueWithCheck(T value) throws BadValueException;
	
	String getValueTmp();
	void setValueTmp(String valueTmp);
	
	Float getValueTmpAsFloat();
	Integer getValueTmpAsInteger();
	
	String getTemplateName();
	void setTemplateName(String templateName);
	
	boolean isReadOnly();
	void setReadOnly(boolean readOnly);
	
	boolean isActive();
	void setActive(boolean active);
	
	boolean isCalculated();
	default boolean isNotCalculated() {
		return !isCalculated();
	}
	void setCalculated(boolean calculated);
	
	String getDescriptionField();
	void setDescriptionField(String descriptionField);
	
	String getNameDescriptionByDescriptionField();
	
	Entity getEntity();
	void setEntity(Entity entity);
	
	Set<IEntityFieldEnabledValue> getEnabledValues();
	void setEnabledValues(Set<IEntityFieldEnabledValue> enabledValues);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
	
	String getValueMeasureStr();
	String getValueStr();
	void setValueStr(String value) throws BadValueException;

	Object getValueObj();

	boolean setValueWithNoCheckStr(String value);
	boolean setValueWithNoCheck(T value);
	
	Date getLastDate();
	void setLastDate(Date lastDate);
	
	IEntityFieldEnabledValue getEntityFieldEnabledValueByCurrentValue();
	
	void validateValue(T value) throws IllegalArgumentException;

	IEntityFieldEnabledValue getEntityFieldEnabledValue(T value);

	IEntityFieldEnabledValue getEntityFieldEnabledValueByValueStr(String value);

	boolean isButton();

	String getMeasure();
	
	String getFullName();
	
	public static int compareByIdChain(IEntityField ia1, IEntityField ia2) {
		int result = -1;
		
		result = ia1.getEntity().getDevice().getId().compareTo(ia2.getEntity().getDevice().getId());
		
		if(result == 0) {
			result = ia1.getEntity().getId().compareTo(ia2.getEntity().getId());
		}
		
		if(result == 0) {
			result = ia1.getEntity().getId().compareTo(ia2.getEntity().getId());
		}
		
		return result;
	}

	Set<IEntityFieldEnabledValue> getEntityFieldEnabledValues();

	String getValueStrByEntityFieldEnabledValueDescription();
	
	default Entity getTargetEntity() {
		return this.getEntity();
	}
}
