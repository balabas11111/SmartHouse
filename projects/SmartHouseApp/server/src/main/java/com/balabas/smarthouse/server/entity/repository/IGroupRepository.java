package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IGroup;

@Repository
public interface IGroupRepository extends CrudRepository<Group, Long>{

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM Group g where g.device.id IN(:id ) ")
	void deleteGroupByDeviceId(@Param("id") Long deviceId);

	@Query("FROM Group where virtualized = true")
	List<IGroup> loadVirtualized();

	@Query("FROM Group where id = :id")
	IGroup loadGroupById(@Param("id") Long id);
}
