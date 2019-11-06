package com.balabas.smarthouse.server.entity.repository;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Group;

@Repository
public interface IGroupRepository extends CrudRepository<Group, Long>{

	@Transactional
	@Modifying
	@Query(value = "DELETE Group WHERE device.id = :id")
	void deleteGroupByDeviceId(@Param("id") Long deviceId);
}
