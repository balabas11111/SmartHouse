package com.balabas.smarthouse.server;

import java.io.IOException;
import java.net.URL;

import org.json.JSONObject;
import org.junit.jupiter.api.Test;
import org.junit.runner.RunWith;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.SmartHouseEntityBuilder;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.common.base.Charsets;
import com.google.common.io.Resources;

import org.junit.platform.runner.JUnitPlatform;

import lombok.extern.log4j.Log4j2;

@Log4j2
@RunWith(JUnitPlatform.class)
public class DeviceBuildFromJsonTest {
	
	@Test
	void testBuildDeviceFromJson() throws IOException{
		URL url = Resources.getResource("examples/dataSwg.json");
		String text = Resources.toString(url, Charsets.UTF_8);
		
		JSONObject deviceJson = new JSONObject(text);
		
		IDevice device = new Device(); 
		/*SmartHouseEntityBuilder.bui
		
		ObjectMapper mapper = new ObjectMapper();
		
		String deviceStr = mapper.writeValueAsString(device);
		*/
		//log.info(deviceStr);
	}
}
