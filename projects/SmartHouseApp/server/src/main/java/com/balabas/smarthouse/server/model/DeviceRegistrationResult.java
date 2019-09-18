package com.balabas.smarthouse.server.model;

public class DeviceRegistrationResult {

	public enum DeviceRegistrationResults {
		UNKNOWN, SUCCESS, FAILED, REJECTED, MANUAL_REJECTED, WAITS, ALREADY_REGISTERED, NOT_ALLOWED
	}

	private boolean registered;
	private DeviceRegistrationResults result;

	public DeviceRegistrationResult() {
		this.setResult(DeviceRegistrationResults.UNKNOWN);
	}

	public DeviceRegistrationResults getResult() {
		return result;
	}

	public void setResult(DeviceRegistrationResults result) {
		this.result = result;

		this.registered = (result != null && (result.equals(DeviceRegistrationResults.SUCCESS)
				|| result.equals(DeviceRegistrationResults.WAITS)));
	}

	public boolean isOkOrUnknown() {
		return isOk() || this.result.equals(DeviceRegistrationResults.UNKNOWN);
	}
	
	public boolean isOk() {
		return this.registered;
	}
}
