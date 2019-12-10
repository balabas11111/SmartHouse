package com.balabas.smarthouse.server.entity.repository;


import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarm.AlarmAbstractEntityField;

@SuppressWarnings("rawtypes")
@Repository
public interface IEntityFieldAlarmRepository extends CrudRepository<AlarmAbstractEntityField, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_alarm_entity_field WHERE id=:id", nativeQuery = true)
	void deleteNativeById(@Param("id")Long entityFieldAlarm);
	
	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_alarm_entity_field "
			+ "WHERE entity_field_id IN (SELECT ef.id AS id from entity_field ef"
			+ " JOIN entities e ON ef.entity_id = e.id"
			+ " JOIN groups g ON g.id = e.group_id"
			+ " JOIN devices d ON d.id = g.device_id"
			+ " WHERE d.id = :id)", nativeQuery = true)
	void deleteEntityFieldAlarmByDeviceId(@Param("id")Long deviceId);
}