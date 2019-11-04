package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

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
}
