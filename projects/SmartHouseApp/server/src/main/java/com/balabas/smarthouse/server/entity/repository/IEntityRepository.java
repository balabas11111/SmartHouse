package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IEntity;

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
	@Query(value = "DELETE FROM Entity e where e.group.id IN(SELECT g.id FROM Group g where g.device.id = :id ) ")
	void deleteEntityByDeviceId(@Param("id") Long deviceId);

	@Query("FROM Entity where virtualized = true")
	List<IEntity> loadVirtualized();

	@Query("FROM Entity where id = :id")
	IEntity loadEntityById(@Param("id") Long id);

}
