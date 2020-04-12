package com.balabas.smarthouse.server.entity.alarmV2;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface AlarmStateChangeEntityRepository extends CrudRepository<AlarmStateChangeEntity, Long> {

}
