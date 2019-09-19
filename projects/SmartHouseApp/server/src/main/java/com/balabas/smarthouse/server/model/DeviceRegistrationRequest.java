package com.balabas.smarthouse.server.model;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DeviceRegistrationRequest {

	private Device device;
	
	private String keyHash;
}
