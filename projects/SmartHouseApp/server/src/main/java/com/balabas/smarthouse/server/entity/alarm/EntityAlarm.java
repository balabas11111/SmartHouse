package com.balabas.smarthouse.server.entity.alarm;

import java.util.LinkedList;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.Emoji;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;
import com.balabas.smarthouse.server.entity.model.descriptor.ActionTimer;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings({ "unchecked", "rawtypes" })
public class EntityAlarm implements IEntityAlarm {

	@JsonIgnore
	@Getter	@Setter
	private IDevice device;

	@JsonIgnore
	@Getter	@Setter
	private IEntity entity;

	@Getter	@Setter
	private String deviceName;

	@Getter	@Setter
	private String entityName;
	
	@Getter	@Setter
	private ActionTimer timer;

	@Getter	@Setter
	private List<IAlarm> alarms = new LinkedList<>();
	
	@Getter	
	boolean notified = false;

	@Override
	public void putEntityFieldAlarm(IAlarm entityFieldAlarm) {
		IAlarm existing = alarms.stream()
				.filter(a -> a.getClass().equals(entityFieldAlarm.getClass())
						&& a.getWatchedItem().getName().equals(entityFieldAlarm.getWatchedItem().getName()))
				.findFirst().orElse(null);

		if(existing == null){
			alarms.add(entityFieldAlarm);
			return;
		}
		
		existing.setValue(entityFieldAlarm.getValue());
	}

	@Override
	public boolean isAlarmed() {
		return alarms.stream().anyMatch(IAlarm::isAlarmed);
	}

	@Override
	public boolean check() {
		boolean alarmed = alarms.stream().map(IAlarm::check).reduce(Boolean::logicalOr).orElse(false);

		if( alarmed && !timer.isActionForced()){
			timer.update(0, true);
		}
		
		return alarmed;
	}

	@Override
	public boolean notificationRequired() {
		return isAlarmed() && !timer.isActionForced() && timer.isTimeToExecuteAction();
	}

	@Override
	public String getAlarmText() {
		if (!isAlarmed()) {
			return null;
		}

		StringBuilder buf = new StringBuilder();

		buf.append(Emoji.ERROR.toString());
		buf.append(" <b>" + entity.getDescription() + "</b>\n");

		alarms.stream().filter(IAlarm::isAlarmed).forEach(a -> buf.append(a.getAlarmText() + "\n"));

		return buf.toString();
	}

	@Override
	public boolean isActive() {
		return device!=null && entity!=null;
		
	}

	@Override
	public void setNotified(boolean notified) {
		timer.update(0, true);
	}

}
