package com.balabas.entities.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.abstr.repository.AbstractRepository;
import com.balabas.entities.model.PhoneNumber;

@Repository
public interface RepositoryPhoneNumber  extends AbstractRepository<PhoneNumber>{

	@Query("from PhoneNumber where name = :name")
	List<PhoneNumber> findByName(@Param("name")String name);
}
