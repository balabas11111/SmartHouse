package com.balabas.smarthouse.server.entity.repository;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@Repository
@SuppressWarnings("rawtypes")
public interface IEntityFieldRepository extends CrudRepository<EntityField, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE EntityField where entity.group.device.id = :id")
	void deleteEntityFieldsByDeviceId(@Param("id") Long deviceId);
	
	@Query("FROM EntityField WHERE id = :id")
	EntityField getEntityFieldbyId(@Param("id")Long entityFieldId);
}
