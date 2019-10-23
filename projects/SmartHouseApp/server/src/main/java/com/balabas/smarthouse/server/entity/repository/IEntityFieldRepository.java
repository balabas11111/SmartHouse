package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@Repository
@SuppressWarnings("rawtypes")
public interface IEntityFieldRepository extends CrudRepository<EntityField, Long> {

}
