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
	@Query(value = "delete from entity_alarm_entity_field where id=:id", nativeQuery = true)
	void deleteNativeById(@Param("id")Long entityFieldAlarm);
}
