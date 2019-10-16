package com.balabas.smarthouse.server.model;

import java.util.Set;

import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Device implements SmartHouseItem/*, JsonDataContainer*/ {

	public enum DeviceState {
		UNKNOWN, CONSTRUCTED, REGISTERED, CONNECTED, FAILED, TIMED_OUT, DISCONNECTED
	};

	private String deviceId;
	private String deviceFirmware;
	private String deviceDescr;

	private String ip;
	private String dataUrl;

	@JsonIgnore
	private String deviceKey;

	private DeviceState state = DeviceState.UNKNOWN;

	private Set<Group> groups;

	@JsonIgnore
	private UpdateTimer timer;

	public Device(String deviceId, long updateInterval) {
		this.deviceId = deviceId;
		timer = new UpdateTimer(updateInterval);
	}

	public boolean isRegistered() {
		return this.state.compareTo(DeviceState.CONSTRUCTED) > 0;
	}

	public boolean isInitialDataReceived() {
		return this.state.compareTo(DeviceState.REGISTERED) > 0;
	}

	public void setDataUpdateCompleted() {
		this.state = DeviceState.CONNECTED;

		timer.setDataReceived();
	}

	public void setDataUpdateFailed() {
		this.state = DeviceState.FAILED;
		timer.setDataUpdateFailed();

		this.getGroups().stream().forEach(group -> getTimer().setDataUpdateFailed());
	}

	public void setDataTimedOut() {
		this.state = DeviceState.TIMED_OUT;
	}
	
	public Group getGroup(String name) {
		return getGroups().stream().filter(group -> group.getName().equals(name)).findFirst().orElse(null);
	}

	public Device updateDevice(Device from) {
		this.deviceDescr = from.deviceDescr!=null?from.deviceDescr:this.deviceDescr;
		this.deviceFirmware = from.deviceFirmware!=null?from.deviceFirmware:this.deviceFirmware;
		this.deviceKey = from.deviceKey!=null?from.deviceKey:this.deviceKey;

		return this;
	}

	@Override
	public String getName() {
		return getDeviceId();
	}

	@Override
	public String getDescription() {
		return getDeviceDescr();
	}
}
