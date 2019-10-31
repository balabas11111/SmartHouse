package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarm.EntityAlarm;

@Repository
public interface IEntityAlarmRepository extends CrudRepository<EntityAlarm, Long> {

	@Query("from EntityAlarm where id = :id")
	EntityAlarm findAlarmById(@Param("id")Long alarmId);
	
	@Query("from EntityAlarm where entity.group.device.id = :id")
	List<EntityAlarm> findAlarmsForDevice(@Param("id")Long deviceId);
}
