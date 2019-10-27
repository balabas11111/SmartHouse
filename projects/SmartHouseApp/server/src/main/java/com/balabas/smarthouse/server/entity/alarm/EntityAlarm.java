package com.balabas.smarthouse.server.entity.alarm;

import java.util.LinkedList;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.FetchType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings({ "unchecked", "rawtypes" })
@javax.persistence.Entity
public class EntityAlarm implements IEntityAlarm {

	@Id
	@Getter	@Setter
	private Long id;

	@Getter	@Setter
	@ManyToOne(targetEntity = Entity.class, fetch = FetchType.EAGER)
	@JoinColumn(name="entity_id", nullable=false)
	private IEntity entity;

	@Transient
	@Getter	@Setter
	private ActionTimer timer;
	
	@Transient
	@Getter @Setter
	private boolean activated;

	@Getter	@Setter
	@OneToMany(targetEntity = AlarmAbstractEntityField.class, mappedBy = "alarm", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	private List<IAlarm> alarms = new LinkedList<>();
	
	@Getter
	@Transient
	boolean alarmed;
	
	@Getter @Setter
	@Transient
	boolean singleAlarmMessage;
	
	//true = alarmMessage, false = finishedMessage, null - nothing
	@Transient
	boolean sendAlarmStartedMessage = false;
	@Transient
	boolean sendAlarmFinishedMessage = false;

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
		if(activated) {
			alarmed = alarms.stream().map(IAlarm::check).reduce(Boolean::logicalOr).orElse(false);

			if (alarmed && !timer.isActionForced()) {
				sendAlarmStartedMessage = true;
				sendAlarmFinishedMessage = false;
				timer.update(0, true);
			} else if (!alarmed && timer.isActionForced()) {
				sendAlarmStartedMessage = false;
				sendAlarmFinishedMessage = true;
				timer.update(0, true);
			}
		}
		return alarmed;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmStarted() {
		//need to send alarm started message
		return timer.isForcedAndTimeToExecute() && sendAlarmStartedMessage;
	}
	
	@JsonIgnore
	@Override
	public boolean isAlarmFinished() {
		//need to send alarm finished message
		return timer.isForcedAndTimeToExecute() && sendAlarmFinishedMessage;
	}
	
	@Override
	public void setAlarmStartedSent(boolean notified) {
		if(notified) {
			if(singleAlarmMessage) {
				sendAlarmStartedMessage = true;
			} else {
				timer.update(timer.getInterval(), true);
			}
		}
	}
	
	@Override
	public void setAlarmFinishedSent(boolean notified) {
		if(notified) {
			sendAlarmFinishedMessage = true;
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
		return getDevice()!=null && entity!=null;
		
	}

	@Override
	public IDevice getDevice() {
		if(getEntity() == null || getEntity().getGroup() == null) {
			return null;
		}
		return getEntity().getGroup().getDevice();
	}

}
