package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;
import java.util.Set;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
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
	
	Float getValueAsFloat();
	
	default float getValueAsfloat() {
		Float result = getValueAsFloat();
		if(result==null) {
			return -1;
		}
		return result.floatValue();
	}
	
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
	
	default boolean isBooleanCommandButton() {
		return !isVirtualized() 
		&& isButton()
		&& !isReadOnly()
		&& getEnabledValues()!=null
		&& getEnabledValues().size()>0;
	}
	
	default boolean isBooleanCommandButtonOfGroup(String groupName) {
		return !isVirtualized() 
		&& isButton()
		&& !isReadOnly()
		&& getEnabledValues()!=null
		&& getEnabledValues().size()>0
		&& groupName.equalsIgnoreCase(getEntity().getGroup().getName());
	}
	
	default boolean isBooleanCommandButtonOfGroupSensors() {
		return 
		isButton()
		&& !isReadOnly()
		&& getEnabledValues()!=null
		&& getEnabledValues().size()>0
		&& DeviceConstants.GROUP_SENSORS.equalsIgnoreCase(getEntity().getGroup().getName());
	}

	String getMeasure();
	
	String getFullName();
	
	default String getItemTypeEmoji() {
		return isBooleanCommandButton()?Emoji.DIM_BUTTON.toString():"";
	}
	
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
	
	static void buildEntityField(IEntityField entityField, Entity entity, Emoji emoji, String name, String description) {
		IItemAbstract.setEmojiNameDescription(entityField, emoji, name, description);
		entityField.setEntity(entity);
	}

}
