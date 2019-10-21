package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Entity;

@Repository
public interface IEntityRepository extends CrudRepository<Entity, Long> {

}
