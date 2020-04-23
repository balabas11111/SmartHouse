package com.balabas.smarthouse.server.entity.alarmV2.model;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.Transient;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;
import lombok.Setter;

@Entity
@SuppressWarnings("rawtypes")
public class AlarmStateChangeAction extends ItemAbstract implements IAlarmStateChangeAction {

	@Setter
	@Enumerated(EnumType.STRING)
	private AlarmState oldState;
	
	@Setter
	@Enumerated(EnumType.STRING)
	private AlarmState newState;
	
	@Getter @Setter
	@Column(name = "interva", nullable = true)
	private Integer interval = -1;
	
	@Getter @Setter
	private String parameter;
	
	@Getter @Setter
	private Long targetFieldId ;
	
	@Getter @Setter
	@Transient
	private IEntityField targetField;
	
	@Override
	public AlarmState getOldState() {
		return Optional.ofNullable(oldState).orElse(AlarmState.NO_DATA);
	}
	
	@Override
	public AlarmState getNewState() {
		return Optional.ofNullable(newState).orElse(AlarmState.NO_DATA);
	}
	
	@Getter @Setter
	private String stringFormatted;
	
	@Getter @Setter
	private String stringParametersFormatted;
	
	@Getter
	@Setter
	boolean disabledIfSameState = false;
	
	@Getter @Setter
	private String targetBeanName;
	
	@Getter 
	final private String hint = "getDescriptionByDescriptionField;getDescription;getName;getValueStr;getValueTmp";
	
	@Override
	public String getAlarmDescription(IItemAbstract item) {
		if(!StringUtils.isEmpty(getStringFormatted())) {
			if(!StringUtils.isEmpty(getStringParametersFormatted())) {
				String[] methodNames = getStringParametersFormatted().split(";");
				
				List<Object> params = new ArrayList<>();
				Object obj = item;
				
				for(String methodName : methodNames) {
					try {
						Method method = obj.getClass().getMethod(methodName);
						Object result = method.invoke(obj);
						params.add(result);
					} catch (Exception e) {
						e.printStackTrace();
					} 
				}
				
				if(params.size()>0) {
					return String.format(getStringFormatted(), params.toArray());
				}
			}
			
			return getStringFormatted();
		}
		
		if(!StringUtils.isEmpty(getDescription())) {
			return getDescription();
		}
		
		return IAlarmV2.getAlarmDescriptionDefault(getOldState(), getNewState());
	}
	
	@Override
	public String executeAction(IItemAbstract item) {
		return getAlarmDescription(item);
	}

	@Override
	public List<IItemAbstract> getAllChildren() {
		return new ArrayList<>();
	}
	
	@Override
	public ItemType getItemType() {
		return ItemType.ALARM;
	}

}
