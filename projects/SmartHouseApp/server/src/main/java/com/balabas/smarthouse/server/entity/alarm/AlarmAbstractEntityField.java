package com.balabas.smarthouse.server.entity.alarm;

import java.util.function.Predicate;

import javax.persistence.FetchType;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.MessageHolder;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings({ "rawtypes", "unchecked" })
@Table(name = "entity_alarm_entity_field")
@javax.persistence.Entity()
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
public abstract class AlarmAbstractEntityField<T> extends Alarm<IEntityField> implements IEntityFieldAlarm<T> {

	@Getter
	@Setter
	@ManyToOne(targetEntity = EntityField.class, fetch = FetchType.EAGER)
	@JoinColumn(name = "entity_field_id", nullable = false)
	private IEntityField watchedItem;

	@Getter
	@Setter
	@ManyToOne(targetEntity = EntityAlarm.class, fetch = FetchType.EAGER)
	@JoinColumn(name = "entity_alarm_id", nullable = false)
	private IEntityAlarm entityAlarm;

	@Transient
	@Getter
	protected Predicate<Integer> predicate;

	@Transient
	protected String compareSeparator;

	public AlarmAbstractEntityField() {
	}

	protected abstract Class<?> getWatchedValueClazz();

	abstract protected Comparable getAlarmValue();

	abstract protected Comparable getEntityFieldValue();

	@Override
	protected boolean executeAlarmChecksInternal() {
		Integer compareValue = getCompareResult();
		return getPredicate().test(compareValue);
	}

	protected Integer getCompareResult() {
		return getAlarmValue().compareTo(getEntityFieldValue());
	}

	@Override
	public MessageHolder getAlarmStartedTextHolder() {
		return new MessageHolder(watchedItem.getEntity().getDevice().getName(), watchedItem.getName(), "");
	}

	@Override
	public String getAlarmText() {
		if (!alarmed) {
			return null;
		}
		return getWatchedItem().getNameDescriptionByDescriptionField() + " =" + getWatchedItem().getValueStr()
				+ getTriggerDescription();
	}

	@Override
	public String getTriggerDescription() {
		return getCompareSeparator() + getValueStr();
	}

	@Override
	public String getValueStr() {
		return getValue().toString();
	}

	@Override
	public boolean acceptsValueAsWatched(Class<?> candidateClass) {
		return getWatchedValueClazz().isAssignableFrom(candidateClass);
	}

	@Override
	public String getCompareSeparator() {
		return compareSeparator != null ? compareSeparator : "";
	}
}
