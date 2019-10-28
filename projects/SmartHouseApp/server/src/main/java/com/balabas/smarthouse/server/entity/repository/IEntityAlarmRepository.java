package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarm.EntityAlarm;

@Repository
public interface IEntityAlarmRepository extends CrudRepository<EntityAlarm, Long> {

}
