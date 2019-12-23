package com.balabas.smarthouse.server.entity.alarmV2;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface IAlarmV2Repository extends CrudRepository<AlarmV2, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_alarm_entity_field WHERE id=:id", nativeQuery = true)
	void deleteNativeById(@Param("id")Long alarmId);
	
}
