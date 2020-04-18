package com.balabas.smarthouse.server.entity.alarmV2.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfEntity;

@Repository
public interface IAlarmV2RepositoryEntity extends CrudRepository<AlarmOfEntity, Long>, IAlarmV2Repository {

}
