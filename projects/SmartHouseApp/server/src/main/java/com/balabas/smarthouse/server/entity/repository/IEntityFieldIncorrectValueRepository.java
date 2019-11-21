package com.balabas.smarthouse.server.entity.repository;

import java.util.Date;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldIncorrectValue;

@Repository
public interface IEntityFieldIncorrectValueRepository extends CrudRepository<EntityFieldIncorrectValue, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE EntityFieldIncorrectValue where entityField.entity.group.device.id = :id")
	void deleteEntityFieldIncorrectValue(@Param("id") Long deviceId);

	@Query("FROM EntityFieldIncorrectValue WHERE id = :id")
	EntityFieldIncorrectValue getEntityFieldIncorrectValueById(@Param("id") Long entityFieldIncorrectValueId);

	@Transactional
	@Modifying
	@Query( value = "INSERT INTO entity_field_incorrect_value(date, value, entity_field_id)"
			+ " VALUES(:date, :value, :entityFieldId)",
			nativeQuery = true)
	void insertEntityFieldIncorrectValue(@Param("entityFieldId") Long entityFieldId, @Param("value") String value,
			@Param("date") Date date);
}
