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
public abstract class Alarm<T extends IItemAbstract>
		implements IAlarm<T> {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter @Setter
	private Long id;
	
	@Setter
	@Transient
	protected boolean alarmed;
	
	@Setter @Getter
	@Transient
	protected boolean activated;

	protected abstract boolean executeAlarmChecksInternal();
	
	public abstract String getAlarmText();
	
	public abstract T getWatchedItem();
	
	public abstract void setWatchedItem(T watchedItem);

	public Alarm() {
	}

	@Override
	public boolean isActive() {
		return isActivated() && this.getWatchedItem() != null;
	}

	@Override
	public boolean isAlarmed() {
		return isActive() && alarmed;
	}

	@Override
	public boolean check() {
		if (!isActive()) {
			return false;
		}
		alarmed = executeAlarmChecksInternal();

		return alarmed;
	}
	
}
