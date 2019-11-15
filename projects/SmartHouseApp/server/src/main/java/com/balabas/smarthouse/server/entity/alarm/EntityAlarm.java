package com.balabas.smarthouse.server.entity.alarm;

import java.util.LinkedList;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.view.MessageHolder;
import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@SuppressWarnings({ "unchecked", "rawtypes" })
@javax.persistence.Entity
@Log4j2
@NoArgsConstructor
public class EntityAlarm extends AlarmRepeatable<IEntity> implements IEntityAlarm {

	@Getter
	@Setter
	@ManyToOne(targetEntity = Entity.class, fetch = FetchType.EAGER)
	@JoinColumn(name = "entity_id", nullable = false)
	private IEntity watchedItem;

	@Getter
	@Setter
	@OneToMany(targetEntity = AlarmAbstractEntityField.class, mappedBy = "entityAlarm", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	private List<IEntityFieldAlarm> alarms = new LinkedList<>();

	public EntityAlarm(IEntity entity) {
		this.watchedItem = entity;
		setMessageInterval(NO_MESSAGE_SEND_REPEATS);
	}
	
	public EntityAlarm(IEntity entity, int messageInterval) {
		this.watchedItem = entity;
		this.messageInterval = messageInterval;
	}
	
	@Override
	public void putAlarm(IEntityFieldAlarm entityFieldAlarm) {
		if(this.getWatchedItem() == null || this.getWatchedItem().getId() ==null
				|| entityFieldAlarm == null
				|| entityFieldAlarm.getWatchedItem() == null
				|| entityFieldAlarm.getWatchedItem().getEntity() == null
				|| !this.getWatchedItem().getId().equals(entityFieldAlarm.getWatchedItem().getEntity().getId())) {
			log.error("Target entity in field is different");
			throw new IllegalArgumentException();
		}
		
		IEntityFieldAlarm existing = alarms.stream()
				.filter(a -> a.getClass().equals(entityFieldAlarm.getClass())
						&& (a.getWatchedItem().getId().equals(entityFieldAlarm.getWatchedItem().getId()) ||
							a.getWatchedItem().getName().equals(entityFieldAlarm.getWatchedItem().getName()))
						&& a.getValue().equals(entityFieldAlarm.getValue()))
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
			checkTimer();
			alarmed = executeAlarmChecksInternal();

			if (alarmed && !getTimer().isActionForced()) {
				updateAlarmState(true);
			} else if (!alarmed && getTimer().isActionForced()) {
				updateAlarmState(false);
			}
			
			if(logAlarmCheck) {
				logAlarmState();
			}
		}
		return alarmed;
	}

	@Override
	public IDevice getDevice() {
		if (getWatchedItem() == null) {
			return null;
		}
		return getWatchedItem().getDevice();
	}
	
	protected boolean executeAlarmChecksInternal() {
		return alarms.stream().map(IAlarm::check).reduce(Boolean::logicalOr).orElse(false);
	}
	
	@Override
	@JsonIgnore
	public MessageHolder getAlarmStartedTextHolder() {
		if (!isAlarmed()) {
			return null;
		}

		MessageHolder result = new MessageHolder(getDevice().getDescription(), getWatchedItem().getDescription(), "", getWatchedItem().getEmoji());
		
		alarms.stream().filter(IAlarm::isAlarmed).forEach(a -> result.addMessage(a.getAlarmText()));

		return result;
	}

	@Override
	@JsonIgnore
	public MessageHolder getAlarmFinishedTextHolder() {
		if (isAlarmed()) {
			return null;
		}

		return new MessageHolder(getDevice().getDescription(), getWatchedItem().getDescription(), ALARM_FINISHED_STATUS, getWatchedItem().getEmoji());
	}
	
	private void updateAlarmState(boolean started) {
		log.warn("Alarm mode "+ (started?"started":"finished") +" " + getDevice().getName() + getWatchedItem().getName());
		sendAlarmStartedMessage = started;
		sendAlarmFinishedMessage = !started;
		getTimer().setNextActionTimeAsNow();
		getTimer().update(0, true);
	}

	private void checkTimer() {
		if(getTimer() == null) {
			setTimer(new ActionTimer(1000 * messageInterval));
		}
	}
	
	private void logAlarmState() {
		StringBuilder buf = new StringBuilder();
		buf.append("Alarm check device=");
		buf.append(getDevice().getName());
		buf.append(" entity=");
		buf.append(getWatchedItem().getName());
		buf.append(" alarmed=");
		buf.append(alarmed);
		buf.append("\n sound=");
		buf.append(isSound());
		buf.append(" activated=");
		buf.append(activated);
		buf.append(" forced=");
		buf.append(getTimer().isActionForced());
		buf.append(" sendAlarmStartedMessage=");
		buf.append(sendAlarmStartedMessage);
		buf.append(" sendAlarmFinishedMessage=");
		buf.append(sendAlarmFinishedMessage);
		buf.append("\n timeTo=");
		buf.append(getTimer().isTimeToExecuteAction());
		
		if(this.getAlarms()!=null) {
			this.getAlarms().stream().forEach( efa ->{
				buf.append(" class=");
				buf.append(efa.getClassSimpleName());
				buf.append(" (");
				buf.append(efa.getCompareSeparator());
				buf.append(" ");
				buf.append(efa.getValueStr());
				buf.append(") watch=");
				buf.append(efa.getWatchedItem().getName());
				buf.append(" val=");
				buf.append(efa.getWatchedItem().getValueStr());
			});
		}
		
		log.info(buf.toString());
	}

	@Override
	public String getAlarmText() {
		return "AlarmText not implemented";
	}

}
