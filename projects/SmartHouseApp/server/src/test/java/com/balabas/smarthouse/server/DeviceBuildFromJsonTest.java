package com.balabas.smarthouse.server;

import java.io.IOException;
import java.net.URL;

import org.json.JSONObject;
import org.junit.Assert;
import org.junit.jupiter.api.Test;
import org.junit.runner.RunWith;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.service.SmartHouseItemBuildService;
import com.google.common.base.Charsets;
import com.google.common.io.Resources;

import org.junit.platform.runner.JUnitPlatform;

@RunWith(JUnitPlatform.class)
public class DeviceBuildFromJsonTest {
	
	@Test
	void testBuildDeviceFromJson() throws IOException{
		SmartHouseItemBuildService buildservice = new SmartHouseItemBuildService();
		URL url = Resources.getResource("examples/dataSwg.json");
		String text = Resources.toString(url, Charsets.UTF_8);
		
		JSONObject deviceJson = new JSONObject(text);
		
		Device device = new Device(); 
		boolean initOk = buildservice.buildDeviceFromJson(device, deviceJson);
		
		Assert.assertTrue("Initialization failed", initOk);
	}
}
