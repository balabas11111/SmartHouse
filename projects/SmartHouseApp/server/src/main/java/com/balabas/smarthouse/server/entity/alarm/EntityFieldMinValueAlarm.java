package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public class EntityFieldMinValueAlarm extends AlarmAbstractEntityFieldNumber {

	public EntityFieldMinValueAlarm(IEntityField field){
		this.watchedItem = field;
	}
	
	@Override
	protected boolean checkItemHasAlarm() {
		Float val = getValue().floatValue();
		Float itemValue = ((Number)this.getWatchedItem().getValue()).floatValue();
		
		return itemValue.compareTo(val) <= 0;
	}

	@Override
	protected String getItemAlarmText() {
		return getWatchedItem().getName() + " " + getWatchedItem().getValueStr() + " < " + getValue().floatValue();
	}

}
