package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldFloat;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

@Service
@SuppressWarnings("rawtypes")
public class VirtualEntityService implements IVirtualEntityService {

	@Autowired
	private IDeviceManageService deviceService;

	protected void setNameDescription(String name, String description, IItemAbstract item) {
		item.setId(0L);
		item.setName(name);
		item.setDescription(description);
		item.setVirtualized(true);
	}
	
	@Override
	public IEntityField createEntityFieldFloat(String name, String description) {
		IEntityField item = new EntityFieldFloat();
		setNameDescription(name, description, item);
		return item;
	}
	
	@Override
	public IEntityField getEntityFieldById(Long id) {
		return deviceService.getEntityFieldById(id);
	}

	@Override
	public IEntity createEntity(String name, String description) {
		IEntity item = new Entity();
		setNameDescription(name, description, item);
		return item;
	}

	@Override
	public IEntity getEntityById(Long id) {
		return deviceService.getEntityById(id);
	}

	@Override
	public IGroup createGroup(String name, String description) {
		IGroup item = new Group();
		setNameDescription(name, description, item);
		return item;
	}

	@Override
	public IGroup getGroupById(Long id) {
		return deviceService.getGroupById(id);
	}

	@Override
	public IDevice createDevice(String name, String description) {
		IDevice item = new Device();
		setNameDescription(name, description, item);
		
		return item;
	}

	@Override
	public IDevice getDeviceById(Long id) {
		return deviceService.getDeviceById(id);
	}

	@Override
	public List<IDevice> getDevices() {
		return deviceService.getDevices().stream().filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public List<IGroup> getGroups() {
		return deviceService.getDevices().stream()
				.flatMap(device -> device.getGroups().stream())
				.filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public List<IEntity> getEntities() {
		return deviceService.getDevices().stream()
				.flatMap(device -> device.getGroups().stream())
				.flatMap(group -> group.getEntities().stream())
				.filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public List<IEntityField> getEntityFields() {
		return deviceService.getDevices().stream()
				.flatMap(device -> device.getGroups().stream())
				.flatMap(group -> group.getEntities().stream())
				.flatMap(entity -> entity.getEntityFields().stream())
				.filter(dev -> dev.isVirtualized()).collect(Collectors.toList());
	}

	@Override
	public void saveDevice(IDevice device) {
		device.setVirtualized(true);
		deviceService.setDeviceAndSave(device);
	}
	
	
/*
	@Override
	public void saveGroup(IGroup group) {
		deviceService.setGroupAndSave(group);
	}

	@Override
	public void saveEntity(IEntity entity) {
		deviceService.setEntityAndSave(entity);		
	}

	@Override
	public void saveEntityField(IEntityField entityField) {
		deviceService.setEntityFieldAndSave(entityField);
	}
	*/
	
}
