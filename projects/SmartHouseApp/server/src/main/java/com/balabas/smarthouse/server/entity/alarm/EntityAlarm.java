package com.balabas.smarthouse.server.entity.alarm;

import java.util.LinkedList;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.Emoji;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
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
	boolean alarmed;
	
	@Getter @Setter
	boolean singleAlarmMessage;
	
	//true = alarmMessage, false = finishedMessage, null - nothing
	Boolean sendMessage;

	@Override
	public void putAlarm(IAlarm entityFieldAlarm) {
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
	public boolean check() {
		alarmed = alarms.stream().map(IAlarm::check).reduce(Boolean::logicalOr).orElse(false);

		if( alarmed && !timer.isActionForced()){
			sendMessage = true;
			timer.update(0, true);
		} else if(!alarmed && timer.isActionForced()) {
			sendMessage = false;
			timer.update(0, true);
		}
		
		return alarmed;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmStarted() {
		//need to send alarm started message
		return timer.isForcedAndTimeToExecute() && Boolean.TRUE.equals(sendMessage);
	}
	
	@JsonIgnore
	@Override
	public boolean isAlarmFinished() {
		//need to send alarm finished message
		return timer.isForcedAndTimeToExecute() && Boolean.FALSE.equals(sendMessage);
	}
	
	@Override
	public void setAlarmStartedSent(boolean notified) {
		if(notified) {
			if(singleAlarmMessage) {
				sendMessage = null;
			} else {
				timer.update(timer.getInterval(), true);
			}
		}
	}
	
	@Override
	public void setAlarmFinishedSent(boolean notified) {
		if(notified) {
			sendMessage = null;
			timer.setActionForced(false);
		}
	}

	@Override
	@JsonIgnore
	public String getAlarmStartedText() {
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
	@JsonIgnore
	public String getAlarmFinishedText() {
		if (isAlarmed()) {
			return null;
		}

		return " <b>" + entity.getDescription() + "</b> - показатели в норме\n";
	}


	@Override
	public boolean isActive() {
		return device!=null && entity!=null;
		
	}

}
