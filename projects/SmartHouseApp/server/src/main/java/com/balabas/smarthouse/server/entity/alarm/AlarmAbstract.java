package com.balabas.smarthouse.server.entity.alarm;

import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public abstract class AlarmAbstract<T extends IItemAbstract, O extends Object>
		implements IAlarm<T, O> {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter @Setter
	private Long id;
	
	@Setter
	@Transient
	private boolean alarmed;
	
	@Setter @Getter
	@Transient
	private boolean activated;

	protected abstract boolean checkItemHasAlarm();

	protected abstract String getItemAlarmText();
	
	public abstract T getWatchedItem();
	
	public abstract void setWatchedItem(T watchedItem);
	
	public abstract O getValue();
	
	public abstract void setValue(O value);

	public AlarmAbstract() {
	}

	public AlarmAbstract(String entityFieldName, O value) {
		this();
		this.setValue(value);
	}

	@Override
	public boolean isActive() {
		return this.activated && this.getValue() != null && this.getWatchedItem() != null;
	}

	@Override
	public boolean isAlarmed() {
		return isActive() && alarmed;
	}

	@Override
	public boolean check() {
		if (!isActive() || getValue() == null || getWatchedItem() == null) {
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
