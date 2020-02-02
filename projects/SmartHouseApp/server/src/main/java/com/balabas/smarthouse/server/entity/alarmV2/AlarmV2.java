package com.balabas.smarthouse.server.entity.alarmV2;

import java.lang.annotation.Annotation;
import java.security.InvalidParameterException;
import java.util.Optional;
import java.util.Set;

import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToMany;
import javax.persistence.MappedSuperclass;
import javax.persistence.Transient;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public abstract class AlarmV2 implements IAlarmV2 {

	public static final Integer NO_MESSAGE_SEND_REPEATS = -1;
	public static final String ALARM_FINISHED_STATUS = "показатели в норме";
	
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter @Setter
	private Long id;
	
	@Getter @Setter
	private String value;
	
	@Getter @Setter
	private Integer messageInterval = NO_MESSAGE_SEND_REPEATS;
	
	@Getter
	@Setter
	@ManyToMany(targetEntity = AlarmStateChangeAction.class, fetch = FetchType.EAGER)
	private Set<IAlarmStateChangeAction> actions;
	
	@Setter
	@Transient
	private AlarmState alarmState;
	
	@Setter
	@Transient
	private AlarmState previousAlarmState;
	
	@Transient
	@Getter @Setter
	boolean alarmStartedSendExpected = false;
	
	@Transient
	@Getter @Setter
	boolean alarmFinishedSendExpected = false;
	
	public abstract Class<?> getItemClass();
	
	public abstract IItemAbstract getItem();
	public abstract void setItem(IItemAbstract item);
	
	public abstract boolean check(IItemAbstract item);
	
	@Override
	public Long getItemId() {
		return getItem()!=null?getItem().getId():null;
	}
	
	@Override
	public AlarmState getAlarmState() {
		return Optional.ofNullable(this.alarmState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public 	AlarmState getPreviousAlarmState() {
		return Optional.ofNullable(this.previousAlarmState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public boolean isInBadState() {
		return this.getAlarmState().badState;
	}
	
	@Override
	public boolean isRepeatable() {
		return !NO_MESSAGE_SEND_REPEATS.equals(messageInterval) && messageInterval > 0;
	}
	
	@Override
	public boolean isValueValid(String value) {
		return !StringUtils.isEmpty(value);
	}

	@SuppressWarnings("rawtypes")
	public static boolean accepts(Class<?> alarmClass, IItemAbstract item) {
		AlarmMarker marker = getAlarmMarker(alarmClass);
		if(IEntityField.class.isAssignableFrom(item.getClass())) {
			IEntityField entityField = (IEntityField) item;
			return marker.target().isAssignableFrom(item.getClass()) && marker.detail().isAssignableFrom(entityField.getClazz());
		}
		
		return marker.target().isAssignableFrom(item.getClass());
	}
	
	@Override
	public IAlarmStateChangeAction getCurrentAction() {
		IAlarmStateChangeAction result = null;
		
		for(IAlarmStateChangeAction action : getActions()) {
			if(action.accepts(getPreviousAlarmState(), getAlarmState())) {
				result = action;
				break;
			}
		}
		
		if(result != null) {
			boolean stateChanged = !getPreviousAlarmState().equals(getAlarmState()); 
			if( stateChanged) {
				if(result.isDispatchIfSameState()) {
					return result;
				}
			} else {
				return result;
			}
			
		}
		
		return null;
	}
	
	public static AlarmMarker getAlarmMarker(Class<?> alarmClass) {
		Annotation annotations[] = alarmClass.getAnnotations();
		
		for(Annotation ann : annotations) {
			if(AlarmMarker.class.isAssignableFrom(ann.getClass())) {
				return (AlarmMarker) ann;
			}
		}
		
		throw new InvalidParameterException();
	}
	
	protected boolean setAlarmStateByState(AlarmState alarmState) {
		AlarmState state1 = getAlarmState();
		
		this.setPreviousAlarmState(getAlarmState());
		this.setAlarmState(Optional.ofNullable(alarmState).orElse(AlarmState.NO_DATA));
		
		return state1.equals(getAlarmState());
	}
	
	protected boolean setAlarmStateByBooleanFlag(boolean alarmed) {
		if(alarmed) {
			return setAlarmStateByState(AlarmState.ALARM);
		} else {
			return setAlarmStateByState(AlarmState.OK);
		}
	}

}
