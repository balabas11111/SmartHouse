package com.balabas.smarthouse.server.entity.alarmV2;


public interface IAlarmV2Repository {

	<T extends IAlarmV2> T save(T entity);
	
	void deleteById(Long id);
}
