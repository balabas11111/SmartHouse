package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.Map.Entry;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.descriptor.DeviceEntityDescriptor;
import com.balabas.smarthouse.server.events.ChangedEvent;
import com.balabas.smarthouse.server.events.EntityEvent;
import com.balabas.smarthouse.server.events.GroupEvent;
import com.balabas.smarthouse.server.events.ValueChangeOperation;
import com.balabas.smarthouse.server.events.ValuesChangeEvent;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceEntity;
import com.balabas.smarthouse.server.model.Group;

import lombok.extern.log4j.Log4j2;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_DEVICE_INFO;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_DESCRIPTION;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SWG;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_COUNT;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_FIELDS_ARRAY;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_KEYS_ARRAY;

import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.ADDED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.INITIAL_DATA_RECEIVED;
import static com.balabas.smarthouse.server.events.ChangedEvent.EventType.UPDATED;

@Log4j2
@Service
@SuppressWarnings("rawtypes")
public class DeviceJsonAdapterBaseImpl implements DeviceJsonAdapter {

	@Override
	public List<ChangedEvent> adapt(JSONObject deviceJson, Device device) {
		log.debug("Adapt data " + deviceJson);

		List<ChangedEvent> events = new ArrayList<>();

		if (device.getGroups() == null) {
			device.setGroups(new HashSet<Group>());
		}

		if (!deviceJson.isEmpty()) {

			processDeviceInfo(device, deviceJson);

			for (String groupName : JSONObject.getNames(deviceJson)) {
				if (ENTITY_FIELD_DESCRIPTION.equals(groupName)) {
					continue;
				}

				Group group = getGroupOrCreateNew(device, groupName, events);

				if (group != null) {
					
					JSONObject groupJson = deviceJson.optJSONObject(groupName);

					if (groupJson == null) {continue;}

					if (!groupJson.isEmpty()) {
						List<EntityEvent> entityEvents = new ArrayList<>();

						for (String entityName : JSONObject.getNames(groupJson)) {

							if (ENTITY_FIELD_DESCRIPTION.equals(entityName)) {
								group.setDescription(groupJson.getString(ENTITY_FIELD_DESCRIPTION));
								continue;
							}

							processEntityJson(!device.isInitialDataReceived(), groupJson, device, group, entityName, entityEvents);
						}

						if (!entityEvents.isEmpty()) {
							events.addAll(entityEvents);

							GroupEvent groupEvent = new GroupEvent(group, UPDATED, entityEvents);
							if (groupEvent != null) {
								events.add(groupEvent);
							}

						}
					}

					group.setDataUpdateCompleted();
				}
			}

			device.setDataUpdateCompleted();
		} else {
			device.setDataUpdateFailed();
		}

		return events;
	}

	private void processDeviceInfo(Device device, JSONObject deviceJson) {
		if (deviceJson.has(DEVICE_FIELD_DEVICE)) {
			JSONObject deviceDeviceJson = deviceJson.getJSONObject(DEVICE_FIELD_DEVICE);

			if (deviceDeviceJson.has(DEVICE_FIELD_DEVICE_INFO)) {
				JSONObject info = deviceDeviceJson.getJSONObject(DEVICE_FIELD_DEVICE_INFO);

				String deviceDescr = info.optString(ENTITY_DEVICE_DEVICE_DESCRIPTION);
				String deviceFirmware = info.optString(ENTITY_DEVICE_DEVICE_FIRMWARE);

				if (!deviceDescr.isEmpty() && !deviceDescr.equals(device.getDescription())) {
					device.setDeviceDescr(deviceDescr);
				}
				if (!deviceFirmware.isEmpty() && !deviceFirmware.equals(device.getDeviceFirmware())) {
					device.setDeviceFirmware(deviceFirmware);
				}
			}

		}
	}

	private Group getGroupOrCreateNew(Device device, String groupName, List<ChangedEvent> events) {
		Group group = device.getGroup(groupName);

		if (group == null) {
			group = new Group(device.getDeviceId(), groupName);
			device.getGroups().add(group);

			if (device.isInitialDataReceived()) {
				events.add(new GroupEvent(group, INITIAL_DATA_RECEIVED));
			}
		}

		return group;
	}

	private void processEntityJson(boolean noEventProduceApplyOnly, JSONObject groupJson, Device device, Group group, String entityName,
			List<EntityEvent> events) {
		JSONObject entityJson = groupJson.optJSONObject(entityName);

		if(entityJson == null) {return;}

		DeviceEntity entity = getEntityOrCreateNew( device, group, entityName, events);

		List<ValuesChangeEvent> valueEvents = new ArrayList<>();

		valueEvents.add(processDeviceEntityJson(noEventProduceApplyOnly, entity, entityJson));

		EntityEvent entityValuesChangeEvent = EntityEvent.build(entity, UPDATED, valueEvents);

		if (entityValuesChangeEvent != null) {
			events.add(entityValuesChangeEvent);
		}
	}

	private DeviceEntity getEntityOrCreateNew(Device device, Group group,
			String entityName, List<EntityEvent> events) {
		DeviceEntity entity = group.getEntity(entityName);

		if (entity == null) {
			entity = new DeviceEntity(group.getDeviceId(), group.getName(), entityName);
			group.getEntities().add(entity);

			events.add(new EntityEvent(device, entity, ADDED));
			events.add(new EntityEvent(device, entity, INITIAL_DATA_RECEIVED));
		}

		return entity;
	}

	private ValuesChangeEvent processDeviceEntityJson(boolean noEventProduceApplyOnly, DeviceEntity entity,
			JSONObject entityJson) {
		List<ValueChangeOperation> changeOperations = new ArrayList<>();

		Map<String, Object> jsonMap = entityJson.toMap();

		for (Entry<String, Object> entry : jsonMap.entrySet()) {
			String key = entry.getKey();

			if (ENTITY_FIELD_SWG.equals(key)) {
				JSONObject descriptorJson = entityJson.optJSONObject(ENTITY_FIELD_SWG);

				log.debug("Entity descriptor : " + descriptorJson);

				Optional.ofNullable(DeviceEntityDescriptor.fromJson(descriptorJson, entity.getName()))
						.ifPresent(entity::setDescriptor);
			} else if (ENTITY_FIELD_ITEM_CLASS.equals(key)) {
				entity.setEntityRenderer(entry.getValue().toString());
			} else if (ENTITY_FIELD_DESCRIPTION.equals(key)) {
				entity.setDescription(entry.getValue().toString());
			} else if (ENTITY_FIELD_SENSOR_ITEMS.equals(key)) {
				JSONObject sensorItemsJson = entityJson.getJSONObject(ENTITY_FIELD_SENSOR_ITEMS);
				log.debug("sensorItemsJson : " + sensorItemsJson);

				processSensorItemsValues(noEventProduceApplyOnly, entity, sensorItemsJson);
			}
			if (!(entry.getValue() instanceof JSONObject) && !(entry.getValue() instanceof JSONArray)) {

				ValueChangeOperation operation = ValueChangeOperation.buildAndApply(entry, entity.getValues());
				if (!noEventProduceApplyOnly && operation.isValueChanged()) {
					changeOperations.add(operation);
				}
			}
		}

		if (changeOperations == null || changeOperations.isEmpty()) {
			return null;
		}

		ValuesChangeEvent event = new ValuesChangeEvent(entity);

		changeOperations.stream().filter(ValueChangeOperation::isValueChanged)
				.forEach(operation -> event.putOperation(operation));

		return event;
	}

	private void processSensorItemsValues(boolean noEventProduceApplyOnly, DeviceEntity entity,
			JSONObject sensorItemsJson) {

		int count = sensorItemsJson.optInt(ENTITY_FIELD_COUNT, 0);

		Set<String> fields = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_FIELDS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		Set<String> keys = Optional.ofNullable(sensorItemsJson.optJSONArray(ENTITY_FIELD_KEYS_ARRAY))
				.orElse(new JSONArray()).toList().stream().map(Object::toString).collect(Collectors.toSet());

		if (count == 0 && !keys.isEmpty()) {
			count = keys.size();
		}

		if (keys.isEmpty() && count > 0) {
			IntStream.range(0, count).forEach(i -> keys.add(Integer.toString(i)));
		}

		if (count == 0 || fields.isEmpty() || keys.size() != count) {
			log.error("Bad SensorItems count=" + count + " fields=" + fields + " keys = " + keys);
		} else {
			entity.setSensorItemFields(fields);
			entity.setSensorItemIds(keys);

			entity.getValues().put(ENTITY_FIELD_COUNT, Integer.toString(count));
			log.debug("Sensor items added");
		}

	}

}
