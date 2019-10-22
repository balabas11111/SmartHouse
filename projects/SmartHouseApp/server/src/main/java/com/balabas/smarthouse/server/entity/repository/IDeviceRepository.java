package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.descriptor.State;

@Repository
public interface IDeviceRepository extends CrudRepository<Device, Long> {

	Device findByName(String name);
	
	@Modifying
	@Query("update Device d set d.state = :state where d.id = :id")
	void updateDeviceState(@Param("id")Long deviceId, @Param("state")State state); 
	
}
