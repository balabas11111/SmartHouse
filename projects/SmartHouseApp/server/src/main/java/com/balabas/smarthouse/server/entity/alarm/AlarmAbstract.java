package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;

import lombok.Getter;
import lombok.Setter;

public abstract class AlarmAbstract<T extends IItemAbstract, O extends Object> extends ItemAbstract implements IAlarm<T, O> {

	@Getter
	@Setter
	private ActionTimer timer;

	@Getter
	@Setter
	protected O value;

	@Getter
	protected T watchedItem;

	@Getter
	@Setter
	private boolean active;

	@Getter
	@Setter
	private boolean alarmed;

	protected abstract boolean checkItemHasAlarm();

	protected abstract String getItemAlarmText();

	public boolean check() {
		if (!active || value == null || watchedItem == null) {
			return false;
		}
		alarmed = checkItemHasAlarm();

		return alarmed;
	}

	public String getAlarmText() {
		if(!alarmed){
			return null;
		}
		
		return getItemAlarmText();
	}

	public String getValueStr() {
		return getValue().toString();
	}

}
