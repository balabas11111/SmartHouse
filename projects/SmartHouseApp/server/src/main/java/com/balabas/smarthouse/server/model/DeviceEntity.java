package com.balabas.smarthouse.server.model;

import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.ToString;

@Data
@NoArgsConstructor
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class DeviceEntity extends ValueContainer {

	@Getter
	private String groupName;

	private Set<String> sensorItemFields;
	private Set<String> sensorItemIds;
	
	@Getter
	private boolean topicSend = false;
	
	public DeviceEntity(String deviceId, String groupName, String entityName) {
		this.deviceId = deviceId;
		this.groupName = groupName;
		this.name = entityName;
	}

	public boolean hasSensorItems() {
		return sensorItemIds != null && !sensorItemIds.isEmpty();
	}

}
