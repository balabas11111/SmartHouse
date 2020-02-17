package com.balabas.entities.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.abstr.repository.AbstractRepository;
import com.balabas.entities.model.PhoneCompany;

@Repository
public interface RepositoryPhoneCompany extends AbstractRepository<PhoneCompany> {

	@Query("from PhoneCompany where name = :name")
	List<PhoneCompany> findByName(@Param("name")String name);
}
