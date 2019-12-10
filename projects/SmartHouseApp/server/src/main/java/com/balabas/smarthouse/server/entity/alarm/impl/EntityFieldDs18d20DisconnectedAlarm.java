package com.balabas.smarthouse.server.entity.alarm.impl;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.alarm.EntityFieldAlarmMarker;

import lombok.ToString;

@Entity
@ToString
@EntityFieldAlarmMarker(target = Number.class)
public class EntityFieldDs18d20DisconnectedAlarm extends AlarmAbstractEntityFieldNumber {

	@Override
	protected boolean executeAlarmChecksInternal() {
		return getValue()!=null && (getValue().intValue() == -127 || getValue().intValue() == 85);
	}

	@Override
	public String getAlarmText() {
		if (!alarmed) {
			return null;
		}
		return getWatchedItem().getNameDescriptionByDescriptionField() + " Датчик отключен ";
	}

}
