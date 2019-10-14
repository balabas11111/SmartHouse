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

	private Set<SensorItem> sensorItems;
	
	@Getter
	private boolean topicSend = false;
	
	public DeviceEntity(String deviceId, String groupName, String entityName) {
		this.deviceId = deviceId;
		this.groupName = groupName;
		this.name = entityName;
	}

	public boolean hasSensorItems() {
		return sensorItems != null && !sensorItems.isEmpty();
	}

	public SensorItem getSensorItem(String sensorItemKey) {
		if (!hasSensorItems()) {
			return null;
		}
		return sensorItems.stream().filter(si -> si.name.equals(sensorItemKey)).findFirst().orElse(null);
	}

}
