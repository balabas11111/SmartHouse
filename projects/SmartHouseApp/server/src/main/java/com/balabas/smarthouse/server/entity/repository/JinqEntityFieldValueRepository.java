package com.balabas.smarthouse.server.entity.repository;

import java.util.Collection;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.EntityFieldValue;

@Repository
public class JinqEntityFieldValueRepository {

	@PersistenceContext
	private EntityManager em;

	@Autowired
	private JinqSource source;
	
	public Collection<EntityFieldValue> getEntityFieldValuesByDeviceName(String deviceName) {
		return source.streamAll(em, EntityFieldValue.class)
				.where(efv -> deviceName.equals(efv.getEntityField().getEntity().getDevice().getName()))
				.toList();
	}
}
