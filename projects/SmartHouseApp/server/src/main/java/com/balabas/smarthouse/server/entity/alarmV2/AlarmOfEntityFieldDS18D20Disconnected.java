package com.balabas.smarthouse.server.entity.alarmV2;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.view.MessageHolder;

@AlarmMarker
@Entity
public class AlarmOfEntityFieldDS18D20Disconnected extends AlarmOfEntityFieldNumber {

	@Override
	public void check(IItemAbstract item) {
		int val = getValueAsFloat().intValue();
		this.alarmed = val == -127 || val == 85;
	}

	public MessageHolder getAlarmStartedTextHolder(IItemAbstract item) {
		MessageHolder holder = getMessageHolder(item, "");
		holder.addMessage("Датчик " + holder.getName() + " отключен");
		return holder;
	}

	@Override
	public MessageHolder getAlarmFinishedTextHolder(IItemAbstract item) {
		return getMessageHolder(item, "Значение датчика в норме");
	}

}
