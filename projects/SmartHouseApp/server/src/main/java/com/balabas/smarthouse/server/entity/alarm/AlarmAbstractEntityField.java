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

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings({ "rawtypes", "unchecked" })
@Table(name = "entity_alarm_entity_field")
@javax.persistence.Entity()
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
public abstract class AlarmAbstractEntityField<T> extends AlarmAbstract<IEntityField, T>
		implements IEntityFieldAlarm<T> {

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
/*
	@Transient
	protected Class<T> clazz;
*/
	@Transient
	@Getter
	protected Predicate<Integer> predicate;

	@Transient
	@Getter
	protected String compareSeparator;

	public AlarmAbstractEntityField() {
		/*Type genericSuperClass = getClass().getGenericSuperclass();

		ParameterizedType parametrizedType = null;
		while (parametrizedType == null) {
			if ((genericSuperClass instanceof ParameterizedType)) {
				parametrizedType = (ParameterizedType) genericSuperClass;
			} else {
				genericSuperClass = ((Class<?>) genericSuperClass).getGenericSuperclass();
			}
		}

		clazz = (Class<T>) parametrizedType.getActualTypeArguments()[0];
		*/
	}
	
	abstract protected Class getWatchedClazz();

	abstract protected Comparable getAlarmValue();

	abstract protected Comparable getEntityFieldValue();

	@Override
	public boolean acceptsAsWatched(IEntityField entityField) {
		return getWatchedClazz().isAssignableFrom(entityField.getClazz());
	}
	
	@Override
	public String getClassSimpleName() {
		return this.getClass().getSimpleName();
	}

	@Override
	protected boolean checkItemHasAlarm() {
		Integer compareValue = getAlarmValue().compareTo(getEntityFieldValue());
		return getPredicate().test(compareValue);
	}

	@Override
	protected String getItemAlarmText() {
		return getWatchedItem().getNameDescriptionByDescriptionField() + " ="
				+ getWatchedItem().getValueStr() + getTriggerDescription();
	}
	@Override
	public String getTriggerDescription() {
		return getCompareSeparator() + getValueStr();
	}
}
