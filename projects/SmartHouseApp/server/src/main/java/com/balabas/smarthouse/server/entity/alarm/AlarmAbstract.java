package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;

import lombok.Getter;
import lombok.Setter;

public abstract class AlarmAbstract<T extends IItemAbstract, O extends Object> extends ItemAbstract
		implements IAlarm<T, O> {

	@Getter	@Setter
	protected O value;

	@Getter	@Setter
	protected T watchedItem;

	@Setter
	private boolean alarmed;

	protected abstract boolean checkItemHasAlarm();

	protected abstract String getItemAlarmText();

	public AlarmAbstract() {
	}

	public AlarmAbstract(String entityFieldName, O value) {
		this();
		this.setName(entityFieldName);
		this.setValue(value);
	}

	@Override
	public boolean isActive() {
		return this.value != null && this.watchedItem != null;
	}

	@Override
	public boolean isAlarmed() {
		return isActive() && alarmed;
	}

	@Override
	public boolean check() {
		if (!isActive() || value == null || watchedItem == null) {
			return false;
		}
		alarmed = checkItemHasAlarm();

		return alarmed;
	}

	@Override
	public String getAlarmText() {
		if (!alarmed) {
			return null;
		}

		return getItemAlarmText();
	}

	public String getValueStr() {
		return getValue().toString();
	}

}
