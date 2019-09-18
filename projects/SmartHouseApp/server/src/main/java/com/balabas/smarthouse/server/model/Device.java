package com.balabas.smarthouse.server.model;

import java.net.InetAddress;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Device {

	private String deviceId;
	private String deviceFirmware;
	private String deviceDescr;
	
	private InetAddress ip;
	private String dataUrl;
	private String rootUrl;
	
	private boolean active;
}
