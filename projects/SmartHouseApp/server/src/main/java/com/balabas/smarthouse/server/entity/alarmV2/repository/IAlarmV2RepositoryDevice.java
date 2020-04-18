package com.balabas.smarthouse.server.entity.alarmV2.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfDevice;

@Repository
public interface IAlarmV2RepositoryDevice extends CrudRepository<AlarmOfDevice, Long>, IAlarmV2Repository {

}
