package com.balabas.smarthouse.server.entity.repository;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Entity;

@Repository
public interface IEntityRepository extends CrudRepository<Entity, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_groupped_fields_ids WHERE entity_id IN "
			+ "(SELECT e.id AS id from entities e "
			+ "JOIN groups g ON g.id = e.group_id "
			+ "JOIN devices d ON d.id = g.device_id WHERE d.id = :id)", nativeQuery = true)
	void deleteEntityGrouppedFieldIdsByDeviceId(@Param("id") Long deviceId);
	
	@Transactional
	@Modifying
	@Query(value = "DELETE FROM entity_groupped_fields_names WHERE entity_id IN "
			+ "(SELECT e.id AS id from entities e "
			+ "JOIN groups g ON g.id = e.group_id "
			+ "JOIN devices d ON d.id = g.device_id WHERE d.id = :id)", nativeQuery = true)
	void deleteEntityGrouppedFieldNamesByDeviceId(@Param("id") Long deviceId);
	
	@Transactional
	@Modifying
	@Query(value = "DELETE Entity WHERE group.device.id = :id")
	void deleteEntityByDeviceId(@Param("id") Long deviceId);
}
