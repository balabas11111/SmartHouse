package com.balabas.smarthouse.server.entity.alarmV2;

import java.lang.annotation.Annotation;
import java.security.InvalidParameterException;

import javax.naming.directory.InvalidAttributesException;
import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
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
	
	@Transient 
	@Getter
	boolean alarmed;
	
	@Transient
	@Getter @Setter
	boolean alarmStartedSendExpected = false;
	
	@Transient
	@Getter @Setter
	boolean alarmFinishedSendExpected = false;
	
	public abstract Class<?> getItemClass();
	
	public abstract IItemAbstract getItem();
	public abstract void setItem(IItemAbstract item);
	
	public abstract void check(IItemAbstract item);
	
	
	
	@Override
	public Long getItemId() {
		return getItem()!=null?getItem().getId():null;
	}
	
	public boolean isValueValid(String value) {
		return !StringUtils.isEmpty(value);
	}

	@Override
	public void check() {
		check(getItem());
	}
	
	public static boolean accepts(Class<?> alarmClass, IItemAbstract item) {
		AlarmMarker marker = getAlarmMarker(alarmClass);
		if(IEntityField.class.isAssignableFrom(item.getClass())) {
			IEntityField entityField = (IEntityField) item;
			return marker.target().isAssignableFrom(item.getClass()) && marker.detail().isAssignableFrom(entityField.getClazz());
		}
		
		return marker.target().isAssignableFrom(item.getClass());
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

}
