package com.balabas.smarthouse.server.entity.repository;

import java.util.Date;
import java.util.List;

import javax.transaction.Transactional;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.EntityFieldValueBoolean;
import com.balabas.smarthouse.server.entity.model.EntityFieldValueNumber;

@Repository
public interface IEntityFieldValueRepository extends CrudRepository<EntityFieldValue, Long>  {

	@Query("FROM EntityFieldValue WHERE entityField.entity.id = :id")
	List<EntityFieldValue> getAllEntityFieldValuesForEntity(@Param("id")Long entityId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.id in(select max(efvi.id) from EntityFieldValue efvi where efvi.entityField.id = :id)")
	List<EntityFieldValue> getLastEntityFieldValuesForEntityField(@Param("id")Long entityFieldId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.entityField.id = :id ORDER BY efv.date DESC")
	List<EntityFieldValue> getEntityFieldValuesForEntityField(@Param("id")Long entityFieldId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.entityField.id = :id AND efv.date > :afterDate ORDER BY efv.date DESC")
	List<EntityFieldValue> getEntityFieldValuesForEntityField(@Param("id")Long entityFieldId, @Param("afterDate") Date date);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.entityField.id = :id AND efv.date > :afterDate AND efv.date < :beforeDate ORDER BY efv.date DESC")
	List<EntityFieldValue> getEntityFieldValuesForEntityField(@Param("id")Long entityFieldId, @Param("afterDate") Date date, @Param("beforeDate") Date beforeDate);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.id in(select max(efvi.id) from EntityFieldValue efvi where efvi.entityField.entity.id = :id group by efvi.entityField.id)")
	List<EntityFieldValue> getLastEntityFieldValuesForEntity(@Param("id")Long entityId);
	
	@Query("SELECT efv FROM EntityFieldValue efv JOIN FETCH efv.entityField WHERE efv.id in(select max(efvi.id) from EntityFieldValue efvi where efvi.entityField.entity.group.device.id = :id group by efvi.entityField.id)")
	List<EntityFieldValue> getLastEntityFieldValuesForDevice(@Param("id")Long deviceId);
	
	@Query("SELECT min(efv.date) FROM EntityFieldValue efv WHERE efv.entityField.id = :id")
	Date selectMinEntityFieldValueDate(@Param("id")Long entityFieldId);
	
	@Query("SELECT max(efv.date) FROM EntityFieldValue efv WHERE efv.entityField.id = :id")
	Date selectMaxEntityFieldValueDate(@Param("id")Long entityFieldId);
	
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
	
	@Transactional
	@Modifying
	@Query( value = "INSERT INTO entity_field_value(dtype, date, value_bool, value_flt, entity_field_id)"
			+ " VALUES('EntityFieldValueNumber', :date, NULL, :value, :entityFieldId)",
			nativeQuery = true)
	void insertEntityFieldValueFloat(@Param("date")Date date, @Param("value")Float value, @Param("entityFieldId")Long entityFieldId);
	
	@Transactional
	@Modifying
	@Query( value = "INSERT INTO entity_field_value(dtype, date, value_bool, value_flt, entity_field_id)"
			+ " VALUES('EntityFieldValueBoolean', :date, :value, NULL, :entityFieldId)",
			nativeQuery = true)
	void insertEntityFieldValueBoolean(@Param("date")Date date, @Param("value")Boolean value, @Param("entityFieldId")Long entityFieldId);
	
	default void saveEntityFieldValue(EntityFieldValue entityFieldValue) {
		if(entityFieldValue.getClass().equals(EntityFieldValueNumber.class)) {
			EntityFieldValueNumber value =(EntityFieldValueNumber) entityFieldValue;
			
			insertEntityFieldValueFloat(new Date(), value.getValueFlt(), value.getEntityField().getId());
		} else
			if(entityFieldValue.getClass().equals(EntityFieldValueBoolean.class)) {
				EntityFieldValueBoolean value =(EntityFieldValueBoolean) entityFieldValue;
				
				insertEntityFieldValueBoolean(new Date(), value.getValueBool(), value.getEntityField().getId());
			}	
			
	}
	
}
