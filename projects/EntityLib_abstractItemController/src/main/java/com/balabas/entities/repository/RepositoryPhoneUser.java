package com.balabas.entities.repository;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import com.balabas.abstr.repository.AbstractRepository;
import com.balabas.entities.model.PhoneUser;

public interface RepositoryPhoneUser extends AbstractRepository<PhoneUser>{

	@Query("from PhoneUser where name = :name")
	List<PhoneUser> findByName(@Param("name")String name);
}
