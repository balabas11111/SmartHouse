package com.balabas.smarthouse.server.entity.service;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Predicate;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.DeviceEntityFieldActionHolder;

@SuppressWarnings({ "rawtypes" })
public interface IDeviceManageService {
	
	Device getManagedDevice(Device device);
	
	List<Device> getDevices();
	
	List<Device> getDevicesInitialized();
	
	void processDataReceivedFromDevice(Device device, String deviceResponse, boolean updateDeviceTimer, boolean updateGroupTimer);
	
	void processDataReceivedFromDevice(Device device, JSONObject data, boolean updateDeviceTimer, boolean updateGroupTimer);
	
	void processDataReceivedFromEntity(Entity entity, JSONObject data);

	void requestDevicesValues(String deviceName);
	
	void requestDevicesValues(Device device, Group group);
	
	boolean processRegistrationRequest(Device device);

	void requestAllDevicesDataWithUpdateRequired();

	String sendDataToDevice(Long deviceId, Long entityId, String actionData);
	String sendDataToDevice(String deviceName, String entityName, Map<String, Object> values);
	String sendDataToDevice(IDevice device, IEntity entity, Map<String, Object> values);

	Device getDeviceById(Long id);
	
	Device getDeviceByName(String name);

	Group getGroupById(Long id);

	Entity getEntityById(Long id);

	IEntityField getEntityFieldById(Long id);

	IEntityFieldEnabledValue getEntityFieldEnabledValueById(Long id);

	Device save(Device device);
	
	IDevice save(IDevice device);
	
	IGroup save(IGroup group);

	IEntity save(IEntity entity);

	IEntityField save(IEntityField entityField);	

	List<Device> getDevicesRequireUpdate();

	List<Group> getGroupsRequireUpdate();

	void reattachAlarmsForDevice(IDevice device);

	List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId);

	List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId);

	List<Entity> getEntitiesForDevice(Long deviceId);

	DeviceEntityFieldActionHolder getValueActionHolder(Long deviceId);

	void deleteDeviceById(Long deviceId);

	List<IEntityField> getCurrentEntityFieldsForDevice(Long deviceId);

	List<Device> getNotRegisteredDevices();

	void requestAllNotRegisteredDevice();

	IEntityFieldService getEntityFieldService();

	void saveEntityFieldValues(List<IEntityField> fields);

	Set<IGroup> getGroups();
	
	Set<IEntity> getEntities();
	
	Set<IEntityField> getEntityFields();
	
	Set<IEntityField> getEntityFields(Predicate<? super IEntityField> filter);
	
	default <T extends IItemAbstract> int getItemIndexById(Collection<T> list, Long id) {
		int result = -1;
		boolean found = false;
		if (id != null) {
			for (T item : list) {
				result++;
				if (id.equals(item.getId())) {
					found = true;
					return result;
				}
			}
		}

		if(!found) {
			result = -1;
		}
		
		return result;
	}

	Set<IDevice> getDevicesNotVirtualized();

	Set<IDevice> getDevices(Predicate<IDevice> pred);

	Set<IEntityField> getCurrentEntityFields();
	
	Set<IEntityField> getCurrentEntityFields(Predicate<IEntityField> pred);

}
