package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.view.MessageHolder;

@AlarmMarker
@Entity
public class AlarmOfEntityFieldNumberMax extends AlarmOfEntityFieldNumber {

	@Override
	public void check(IItemAbstract item) {
		this.alarmed = getEntityFieldValueFloat(item) > getValueAsFloat();
	}
	
	public MessageHolder getAlarmStartedTextHolder(IItemAbstract item) {
		MessageHolder holder = getMessageHolder(item, "");
		holder.addMessage("Датчик " + holder.getName() + " - значение " + getEntityFieldValueFloat(item) + " > " + getValue());
		return holder;
	}

	@Override
	public MessageHolder getAlarmFinishedTextHolder(IItemAbstract item) {
		return getMessageHolder(item, "Значение датчика в норме");
	}

}
