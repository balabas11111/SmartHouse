package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Group;

@Repository
public interface IGroupRepository extends CrudRepository<Group, Long>{

}
