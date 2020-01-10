package com.balabas.smarthouse.server.entity.alarm.impl;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.alarm.EntityFieldAlarmMarker;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.ToString;

@Entity
@ToString
@SuppressWarnings("rawtypes")
@EntityFieldAlarmMarker(target = Number.class)
public class EntityFieldDs18d20DisconnectedAlarm extends AlarmAbstractEntityFieldNumber {

	
	@Override
	protected boolean executeAlarmChecksInternal() {
		IEntityField entityField = getWatchedItem();
		Integer intValue = entityField.getValueTmpAsInteger();
		return intValue!=null && (intValue == -127 || intValue == 85);
	}
	

	@Override
	public String getAlarmText() {
		if (!alarmed) {
			return null;
		}
		return getWatchedItem().getNameDescriptionByDescriptionField() + " Датчик отключен ";
	}

}
