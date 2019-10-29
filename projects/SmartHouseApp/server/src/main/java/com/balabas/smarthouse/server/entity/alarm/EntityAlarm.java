package com.balabas.smarthouse.server.entity.alarm;

import java.util.LinkedList;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@SuppressWarnings({ "unchecked", "rawtypes" })
@javax.persistence.Entity
@Log4j2
@NoArgsConstructor
public class EntityAlarm implements IEntityAlarm {

	public static final int NO_MESSAGE_SEND_REPEATS = -1;
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	@Setter
	private Long id;

	@Getter
	@Setter
	@ManyToOne(targetEntity = Entity.class, fetch = FetchType.EAGER)
	@JoinColumn(name = "entity_id", nullable = false)
	private IEntity entity;

	@Transient
	@Getter
	@Setter
	private ActionTimer timer;

	@Transient
	@Getter
	@Setter
	private boolean activated;

	@Getter
	@Setter
	@OneToMany(targetEntity = AlarmAbstractEntityField.class, mappedBy = "entityAlarm", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	private List<IEntityFieldAlarm> alarms = new LinkedList<>();

	@Getter
	@Transient
	boolean alarmed;

	// if -1 send only one message
	@Getter
	@Setter
	int messageInterval;

	// true = alarmMessage, false = finishedMessage, null - nothing
	@Transient
	boolean sendAlarmStartedMessage = false;
	@Transient
	boolean sendAlarmFinishedMessage = false;

	public EntityAlarm(IEntity entity) {
		this.entity = entity;
		this.messageInterval = NO_MESSAGE_SEND_REPEATS;
	}
	
	public EntityAlarm(IEntity entity, int messageInterval) {
		this.entity = entity;
		this.messageInterval = messageInterval;
	}
	
	@Override
	public void putAlarm(IEntityFieldAlarm entityFieldAlarm) {
		if(this.getEntity() == null || this.getEntity().getId() ==null
				|| entityFieldAlarm == null
				|| entityFieldAlarm.getWatchedItem() == null
				|| entityFieldAlarm.getWatchedItem().getEntity() == null
				|| !this.getEntity().getId().equals(entityFieldAlarm.getWatchedItem().getEntity().getId())) {
			log.error("Target entity in field is different");
			throw new IllegalArgumentException();
		}
		
		IEntityFieldAlarm existing = alarms.stream()
				.filter(a -> a.getClass().equals(entityFieldAlarm.getClass())
						&& a.getWatchedItem().getName().equals(entityFieldAlarm.getWatchedItem().getName()))
				.findFirst().orElse(null);

		if (existing == null) {
			log.info("New entityFieldAlarm added " + entityFieldAlarm.toString());
			alarms.add(entityFieldAlarm);
			return;
		}

		if (!existing.getValue().equals(entityFieldAlarm.getValue())) {
			existing.setValue(entityFieldAlarm.getValue());
			log.info("entityFieldAlarm updated " + entityFieldAlarm.toString());
		} else {
			log.warn("entityFieldAlarm no changes");
		}
	}

	@Override
	public boolean check() {
		if (activated) {
			alarmed = alarms.stream().map(IAlarm::check).reduce(Boolean::logicalOr).orElse(false);

			if (alarmed && !timer.isActionForced()) {
				updateAlarmState(true);
			} else if (!alarmed && timer.isActionForced()) {
				updateAlarmState(false);
			}
		}
		return alarmed;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmStarted() {
		// need to send alarm started message
		return activated && timer.isForcedAndTimeToExecute() && sendAlarmStartedMessage;
	}

	@JsonIgnore
	@Override
	public boolean isAlarmFinished() {
		// need to send alarm finished message
		return activated && timer.isForcedAndTimeToExecute() && sendAlarmFinishedMessage;
	}

	@Override
	public void setAlarmStartedSent(boolean notified) {
		if (notified) {
			if (messageInterval == -1) {
				sendAlarmStartedMessage = false;
			} else {
				timer.update(timer.getInterval(), true);
			}
		}
	}

	@Override
	public void setAlarmFinishedSent(boolean notified) {
		if (notified) {
			sendAlarmFinishedMessage = false;
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

		buf.append(entity.getEmoji().toString());
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
		return getDevice() != null && entity != null;

	}

	@Override
	public IDevice getDevice() {
		if (getEntity() == null || getEntity().getGroup() == null) {
			return null;
		}
		return getEntity().getGroup().getDevice();
	}
	
	private void updateAlarmState(boolean started) {
		log.warn("Alarm mode "+ (started?"started":"finished") +" " + entity.getGroup().getDevice().getName() + entity.getName());
		sendAlarmStartedMessage = started;
		sendAlarmFinishedMessage = !started;
		timer.setNextActionTimeAsNow();
		timer.update(0, true);
	}

}
