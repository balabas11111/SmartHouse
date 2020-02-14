package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface IAlarmV2RepositoryEntity extends CrudRepository<AlarmOfEntity, Long>, IAlarmV2Repository {

}
