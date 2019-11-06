package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.EntityFieldValue;

@Repository
public interface IEntityFieldValueRepository extends CrudRepository<EntityFieldValue, Long>  {

	@Query("FROM EntityFieldValue WHERE entityField.entity.id = :id")
	List<EntityFieldValue> getAllEntityFieldValuesForEntity(@Param("id")Long entityId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.id in(select max(efvi.id) from EntityFieldValue efvi where efvi.entityField.entity.id = :id group by efvi.entityField.id)")
	List<EntityFieldValue> getLastEntityFieldValuesForEntity(@Param("id")Long entityId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.id in(select max(efvi.id) from EntityFieldValue efvi where efvi.entityField.entity.group.device.id = :id group by efvi.entityField.id)")
	List<EntityFieldValue> getLastEntityFieldValuesForDevice(@Param("id")Long deviceId);
	
	@Transactional
	@Modifying
	@Query( value = "DELETE FROM entity_field_value" + 
			" WHERE entity_field_id in" + 
			"	(SELECT ef.id AS id from entity_field ef" + 
			"	JOIN entities e ON ef.entity_id = e.id" + 
			"	JOIN groups g ON g.id = e.group_id" + 
			"	JOIN devices d ON d.id = g.device_id" + 
			"	WHERE d.id = :id)", nativeQuery = true)
	void deleteEntityFieldValuesForDevice(@Param("id")Long deviceId);
}
