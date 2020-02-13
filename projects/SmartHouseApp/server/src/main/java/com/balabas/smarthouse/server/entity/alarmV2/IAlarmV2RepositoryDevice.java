package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface IAlarmV2RepositoryDevice extends CrudRepository<AlarmOfDevice, Long>, IAlarmV2Repository {

}
