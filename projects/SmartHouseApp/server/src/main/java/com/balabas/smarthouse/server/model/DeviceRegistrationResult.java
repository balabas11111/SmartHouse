package com.balabas.smarthouse.server.model;

public class DeviceRegistrationResult {

	public enum DeviceRegistrationStatus {
		UNKNOWN, SUCCESS, FAILED, REJECTED, MANUAL_REJECTED, WAITS, ALREADY_REGISTERED, NOT_ALLOWED, FORBIDDEN
	}

	private boolean registered;
	private DeviceRegistrationStatus result;

	public DeviceRegistrationResult() {
		this.setResult(DeviceRegistrationStatus.UNKNOWN);
	}
	
	public DeviceRegistrationResult(DeviceRegistrationStatus status) {
		this.setResult(status);
	}

	public DeviceRegistrationStatus getResult() {
		return result;
	}

	public void setResult(DeviceRegistrationStatus result) {
		this.result = result;

		this.registered = (result != null && (result.equals(DeviceRegistrationStatus.SUCCESS)
				|| result.equals(DeviceRegistrationStatus.WAITS)));
	}

	public boolean isOkOrUnknown() {
		return isOk() || this.result.equals(DeviceRegistrationStatus.UNKNOWN);
	}
	
	public boolean isOk() {
		return this.registered;
	}
	
	public static DeviceRegistrationResult build(DeviceRegistrationStatus status){
		return new DeviceRegistrationResult(status);
	}
}
