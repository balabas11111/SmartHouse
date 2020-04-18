package com.balabas.smarthouse.server.entity.alarmV2.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEntity;

@Repository
public interface AlarmStateChangeEntityRepository extends CrudRepository<AlarmStateChangeEntity, Long> {

}
