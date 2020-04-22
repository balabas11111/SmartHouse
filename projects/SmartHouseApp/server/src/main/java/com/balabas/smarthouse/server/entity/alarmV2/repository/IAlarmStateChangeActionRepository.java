package com.balabas.smarthouse.server.entity.alarmV2.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmStateChangeAction;

@Repository
public interface IAlarmStateChangeActionRepository extends CrudRepository<AlarmStateChangeAction, Long> {

}
