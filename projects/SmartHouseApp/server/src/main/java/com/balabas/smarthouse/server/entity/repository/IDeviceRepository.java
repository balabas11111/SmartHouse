package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.entity.model.Device;

@Repository
public interface IDeviceRepository extends CrudRepository<Device, Long> {

	Device findByName(String name);
}
