package com.balabas.smarthouse.server.entity.alarmV2;

import java.lang.annotation.Annotation;

import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;
import javax.persistence.Transient;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public abstract class AlarmV2 implements IAlarmV2 {

	public static final Integer NO_MESSAGE_SEND_REPEATS = -1;
	
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
	
	@Override
	public Long getItemId() {
		return getItem()!=null?getItem().getId():null;
	}

	@Override
	public Class<?> getItemClass() {
		Annotation annotations[] = this.getClass().getAnnotations();
		
		for(Annotation ann : annotations) {
			if(AlarmMarker.class.isAssignableFrom(ann.getClass())) {
				AlarmMarker marker = (AlarmMarker) ann;
				return marker.target();
			}
		}
		
		throw new IllegalAccessError("No AlarmMarker specified");	
	}
	
	@Override
	public boolean accepts(IItemAbstract item) {
		try {
			return getItemClass().isAssignableFrom(item.getClass());
		}catch(Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean isValueValid(String value) {
		return !StringUtils.isEmpty(value);
	}

	public abstract IItemAbstract getItem();
	public abstract void setItem(IItemAbstract item);
	
	public abstract void check();
	
	public static boolean accepts(Class<?> acceptor, IItemAbstract item) {
		Annotation annotations[] = acceptor.getAnnotations();
		
		for(Annotation ann : annotations) {
			if(AlarmMarker.class.isAssignableFrom(ann.getClass())) {
				AlarmMarker marker = (AlarmMarker) ann;
				return marker.target().isAssignableFrom(item.getClass());
			}
		}
		
		return false;
	}

}
