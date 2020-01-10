package com.balabas.smarthouse.server.entity.repository;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.virtual.CalculatedEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.ICalculatedEntityField;

@Repository
public interface ICalculatedEntityFieldRepository extends CrudRepository<CalculatedEntityField, Long> {

	@Transactional
	@Modifying
	@Query(value = "DELETE FROM CalculatedEntityField ef where ef.targetEntityField.id IN(:id)")
	void deleteCalculatedEntityFieldsByTargetId(@Param("id") Long id);
	
	@Query("FROM CalculatedEntityField WHERE id = :id")
	ICalculatedEntityField getEntityFieldbyId(@Param("id")Long entityFieldId);
}
