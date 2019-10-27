package com.balabas.smarthouse.server.entity.alarm;

import java.util.function.Predicate;

import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings( {"rawtypes"})
@MappedSuperclass
public abstract class AlarmAbstractEntityFieldNumber extends AlarmAbstractEntityField<Number> {

	private Number number;
	
	public AlarmAbstractEntityFieldNumber(Predicate<Integer> predicate, String compareSeparator){
		this.predicate = predicate;
		this.compareSeparator = compareSeparator;
	}
		
	public AlarmAbstractEntityFieldNumber(IEntityField entityField, Number value, 
			Predicate<Integer> predicate, String compareSeparator){
		this(predicate, compareSeparator);
		
		if(value == null || entityField == null || !acceptsAsWatched(entityField)) {
			throw new IllegalArgumentException("Bad value for entityField check ");
		}
		
		this.setValue(value);
		this.setWatchedItem(entityField);
	}
	
	@Override 
	public Number getValue() {
		return number;
	}
	
	@Override 
	public void setValue(Number number) {
		this.number = number;
	}
	
	public Comparable getAlarmValue() {
		return getValue().floatValue();
	}
	
	public Comparable getEntityFieldValue() {
		return ((Number)this.getWatchedItem().getValue()).floatValue();
	}
}
