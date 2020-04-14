package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.List;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IAlarmV2 extends IIdentifiable, IDescriptionable {

	default Emoji getEmoji() {
		List<IAlarmStateChangeAction> actions = getCurrentActions();
		
		Emoji result = getAlarmState().getEmoji();
		
		if(actions!=null && !actions.isEmpty()) {
			for(IAlarmStateChangeAction action : actions) {
				if(!action.getEmoji().isEmpty()) {
					result = action.getEmoji();
					break;
				}
			}
		}
		return result;
	}
	
	default String getEmojiStr() {
		return getEmoji().toString();
	}
	
	Long getItemId();
	
	Class<?> getTargetItemClass();
	
	ItemType getItemType();
	
	IItemAbstract getItem();
	void setItem(IItemAbstract item);
	
	AlarmState getAlarmState();
	void setAlarmState(AlarmState alarmState);
	
	List<String> getStateDescriptions();
	void setStateDescriptions(List<String> list);
	
	default boolean hasStateDescriptions() {
		List<String> list = getStateDescriptions();
		return list!=null && !list.isEmpty();
	}
	
	AlarmState getPreviousAlarmState();
	void setPreviousAlarmState(AlarmState previousAlarmState);
	
	String getParameter();
	void setParameter(String parameter);
	
	boolean isInBadState();
	boolean isRepeatable();

	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	Set<IAlarmStateChangeAction> getActions();
	void setActions(Set<IAlarmStateChangeAction> actions);
	
	List<IAlarmStateChangeAction> getCurrentActions();

	static String getAlarmDescriptionDefault(AlarmState oldState, AlarmState newState) {
		return String.format(AlarmConstants.DEFAULT_DESCRIPTION, oldState.getDescription(), newState.getDescription());
	}
	
	String getCheckerName();
	void setCheckerName(String checkerName);
	
	default boolean isParameterValid(String value) {
		return true;
	}
	
	default boolean isSameStateAction() {
		return getAlarmState().equals(getPreviousAlarmState());
	}
	
	default String getUid() {
		return "Alarm_" + getItem().getItemClassId() + "_" + getId();
	}
	
	default String getAlarmStateEmojiDescription() {
		return getAlarmState().getEmojiDescription();
	}

	boolean setAlarmStateByState(AlarmState alarmState);

	List<IAlarmStateChangeAction> getActionsList();

	boolean isStateChanged();

	String getStateDescriptionsWithNextLine();
	
	boolean setAlarmStateByBooleanFlagAlarm(boolean alarmed);
	
	boolean setAlarmStateByBooleanFlagWarning(boolean alarmed);

	boolean setAlarmStateByBooleanFlag(boolean alarmed, AlarmState stateTrue, AlarmState stateFalse);

	

	

}
