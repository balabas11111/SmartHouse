package com.balabas.smarthouse.server.entity.alarmV2.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarmV2.repository.AlarmStateChangeEntityRepository;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class AlarmStateChangeEntityService implements IAlarmStateChangeEntityService {
	
	@Getter @Setter
	@Autowired
	AlarmStateChangeEntityRepository alarmStateChangeEntityRepository;

	@Override
	public void log(String string) {
		log.info(string);
	}

}
