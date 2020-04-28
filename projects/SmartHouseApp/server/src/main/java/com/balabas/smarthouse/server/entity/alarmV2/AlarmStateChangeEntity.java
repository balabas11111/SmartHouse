package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import javax.persistence.Entity;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.model.IItemTypeable;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.Getter;
import lombok.Setter;

@Entity
public class AlarmStateChangeEntity implements IItemTypeable {

	@Getter
	private final ItemType itemType = ItemType.ALARM_STATE;
	
	@Getter @Setter
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Long id = 0L;
	
	@Getter @Setter
	@Enumerated(EnumType.STRING)
	private ItemType type;
	
	@Getter @Setter
	private Long targetAlarmId;
	
	@Getter @Setter
	private Date dateTime;
	
	@Getter @Setter
	@Enumerated(EnumType.STRING)
	private AlarmState alarmState;
	
	@Getter @Setter
	private Integer valueInt;
	
}
