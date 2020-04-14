package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.util.MathUtil;

@SuppressWarnings("rawtypes")
public abstract class AlarmV2CheckerAbstractEntityFieldNumber {

	protected boolean checkItemValue(IAlarmV2 alarm) {
		return false;
	}
	
	public Class<?> getItemClass() {
		return IEntityField.class;
	}
	
	public void process(IAlarmV2 alarm) {
		alarm.setAlarmStateByBooleanFlagAlarm(checkItemValue(alarm));
	}
	
	public boolean isParameterValid(String value) {
		return MathUtil.isValidFloat(value);
	}
	
	protected IEntityField getItemAsEntityField(IAlarmV2 alarm) {
		return (IEntityField) alarm.getItem();
	}
	
	protected Float getValueTmpAsFloat(IEntityField entityField) {
		return entityField.getValueTmpAsFloat();
	}
	
	protected Integer getValueTmpAsInteger(IEntityField entityField) {
		return Integer.valueOf(entityField.getValueTmpAsInteger());
	}
	
	protected Float getEntityFieldValueFloat(IItemAbstract item) {
		IEntityField entityField = (IEntityField) item;
		return getEntityFieldValueFloat(entityField);
	}
	
	protected Float getEntityFieldValueFloat(IEntityField entityField) {
		return ((Number) entityField.getValue()).floatValue();
	}
	
	protected Long getAlarmParameterAsLongOrElse(IAlarmV2 alarm, Long defaul) {
		Long interval = defaul;
		String params = alarm.getParameter();
		if(MathUtil.isValidFloat(params)) {
			interval = MathUtil.getAsFloat(params).longValue();
		}
		return interval;
	}
	

}
