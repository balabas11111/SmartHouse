package com.balabas.smarthouse.server.device;

import lombok.Getter;
import lombok.Setter;

public class DeviceFile {

	@Getter @Setter
	private String ip;
	@Getter @Setter
	private String name;
	@Getter @Setter
	private String path;
	@Getter @Setter
	private int size;
	
	public DeviceFile(String ip, String filePathName, int size) {
		int ind = filePathName.lastIndexOf("/");
		
		if(ind == -1) {
			ind = filePathName.lastIndexOf("\\");
		}
		
		if(ind!=-1) {
			this.name = filePathName.substring(ind+1);
			this.path = filePathName;
		} else {
			this.path = filePathName;
			this.name = filePathName;
			
		}
		
		this.ip = ip;
		this.size = size;
	}
}
