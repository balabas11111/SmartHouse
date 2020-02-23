package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmV2 extends IIdentifiable, IDescriptionable {

	Long getItemId();
	//void setItemId(Long itemId);
	
	Class<?> getTargetItemClass();
	
	IItemAbstract getItem();
	void setItem(IItemAbstract item);
	
	AlarmState getAlarmState();
	void setAlarmState(AlarmState alarmState);
	
	AlarmState getPreviousAlarmState();
	void setPreviousAlarmState(AlarmState previousAlarmState);
	
	String getParameter();
	void setParameter(String parameter);
	
	boolean isInBadState();
	boolean isRepeatable();
	
	AlarmV2Checker getChecker();
	void setChecker(AlarmV2Checker checker);
	
	boolean check(IItemAbstract item);
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	Set<IAlarmStateChangeAction> getActions();
	void setActions(Set<IAlarmStateChangeAction> actions);
	
	List<IAlarmStateChangeAction> getCurrentActions();

	static String getAlarmDescriptionByState(IAlarmStateChangeAction action, IItemAbstract item) {
		if(action!=null) {
			return action.getAlarmDescription(item);
		} else {
			return getAlarmDescriptionDefault();
		}
	}
	
	static String getAlarmDescriptionDefault() {
		StringBuilder builder = new StringBuilder();
		
		
		return builder.toString();
	}
	
	String getCheckerName();
	void setCheckerName(String checkerName);
	
	default boolean isParameterValid(String value) {
		return true;
	}
	
	default boolean check() {
		return check(getItem());
	}

	default boolean isSameStateAction() {
		return getAlarmState().equals(getPreviousAlarmState());
	}

	boolean setAlarmStateByState(AlarmState alarmState);

	boolean setAlarmStateByBooleanFlag(boolean alarmed);

	List<IAlarmStateChangeAction> getActionsList();

}
