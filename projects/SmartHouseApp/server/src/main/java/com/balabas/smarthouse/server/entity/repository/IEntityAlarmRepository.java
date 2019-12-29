package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarm.AlarmAbstractEntityField;
import com.balabas.smarthouse.server.entity.alarm.EntityAlarm;
import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;

@Repository
public interface IEntityAlarmRepository extends CrudRepository<EntityAlarm, Long> {

	@Query("from EntityAlarm where id = :id")
	EntityAlarm findAlarmById(@Param("id")Long alarmId);
	
	@Query("from EntityAlarm where watchedItem.group.device.id = :id")
	List<EntityAlarm> findAlarmsForDevice(@Param("id")Long deviceId);
	
	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_alarm" + 
			" WHERE entity_id IN" + 
			"	(SELECT e.id AS id from entities e" + 
			"	JOIN groups g ON g.id = e.group_id" + 
			"	JOIN devices d ON d.id = g.device_id" + 
			"	WHERE d.id = :id)", nativeQuery = true)
	void deleteEntityAlarmsByDeviceId(@Param("id") Long deviceId);

	@Query("from AlarmAbstractEntityField where watchedItem.id = :id")
	List<AlarmAbstractEntityField> findAlarmsForEntityField(@Param("id")Long entityFieldId);
}
