package com.balabas.smarthouse.server.entity.repository;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValue;

@SuppressWarnings("rawtypes")
@Repository
public interface IEntityFieldEnabledValueRepository extends CrudRepository<EntityFieldEnabledValue, Long>  {

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM EntityFieldEnabledValue efev where efev.entityField.id IN(SELECT ef.id FROM EntityField ef where ef.entity.group.device.id = :id ) ")
	void deleteEntityFieldEnabledValuesForDevice(@Param("id")Long deviceId);
}
