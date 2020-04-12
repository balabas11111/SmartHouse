package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.IItemTypeable;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

import lombok.Getter;
import lombok.Setter;

@Entity
public class AlarmStateChangeEntity implements IItemTypeable, IDescriptionable {

	@Getter @Setter
	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	private Long id = 0L;
	
	@Getter @Setter
	private ItemType type;
	
	@Getter @Setter
	private Long targetAlarmId;
	
	@Getter @Setter
	private Date dateTime;
	
	@Getter @Setter
	private AlarmState alarmState;
	
	@Getter @Setter
	private Integer valueInt;
	
	@Getter @Setter
	private String description;
}
