package com.balabas.smarthouse.server.security;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.ToString;

@Data
@ToString
@AllArgsConstructor
public class DeviceSecurityContext {

	private String serverKeyHash;
	private String deviceKeyHash;
	private String serverToken;
	
}
