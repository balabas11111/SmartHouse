package com.balabas.smarthouse.server.entity.alarmV2.model;

import java.util.List;
import java.util.Set;

import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmConstants;
import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IAlarmV2 extends IIdentifiable, IDescriptionable {

	default Emoji getEmoji() {
		List<IAlarmStateChangeAction> actions = getCurrentActions();
		
		Emoji result = getAlarmState().getEmoji();
		
		if(actions!=null && !actions.isEmpty()) {
			for(IAlarmStateChangeAction action : actions) {
				if(!action.getEmoji().isEmpty()) {
					return action.getEmoji();
				}
			}
		}
		return result;
	}
	
	default String getCurrentActionEmojiDescription() {
		List<IAlarmStateChangeAction> actions = getCurrentActions();
		
		if(actions!=null && !actions.isEmpty()) {
			for(IAlarmStateChangeAction action : actions) {
				if(!action.getEmoji().isEmpty()) {
					String result = null;
					if(!StringUtils.isEmpty(action.getDescription())) {
						result = action.getDescription();
					} else if(!StringUtils.isEmpty(action.getStringFormatted())) {
						result = action.getStringFormatted();
					}
					
					if(result !=null) {
						return action.getEmoji().toString() + " " + result;
					}
					return action.getEmojiDescriptionByDescriptionField();
				}
			}
		}
		return getAlarmStateEmojiDescription();
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
	
	static int compareByDescription(IAlarmV2 al1,IAlarmV2 al2) {
		return al1.getDescription().compareTo(al2.getDescription());
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

	ItemAbstractDto getViewDescriptor();
	
	void setViewDescriptor(ItemAbstractDto itemAbstractDto);

	void setStateDescription(String description);

	

	

}
