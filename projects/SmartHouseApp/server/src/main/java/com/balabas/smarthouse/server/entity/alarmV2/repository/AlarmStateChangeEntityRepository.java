package com.balabas.smarthouse.server.entity.alarmV2.repository;

import java.util.Date;
import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEntity;

@Repository
public interface AlarmStateChangeEntityRepository extends CrudRepository<AlarmStateChangeEntity, Long> {

	@Query("SELECT e FROM AlarmStateChangeEntity e WHERE e.targetAlarmId = :id AND e.dateTime > :afterDate AND e.dateTime <= :beforeDate ORDER BY e.dateTime ASC")
	List<AlarmStateChangeEntity> getAlarmStateChangeEntitiesForPeriod(@Param("id")Long targetId, @Param("afterDate")Date date1, @Param("beforeDate")Date date2);

}
