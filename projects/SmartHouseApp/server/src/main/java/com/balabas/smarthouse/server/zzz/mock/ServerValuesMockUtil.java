package com.balabas.smarthouse.server.zzz.mock;

import static com.balabas.smarthouse.server.DeviceConstants.GROUP_DEVICE;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SENSORS;
import static com.balabas.smarthouse.server.DeviceConstants.GROUP_SETTINGS;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_ID;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_DEVICE_DEVICE_FIRMWARE;

import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_DESCRIPTION;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ITEM_CLASS;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_ON;
import static com.balabas.smarthouse.server.DeviceConstants.ENTITY_FIELD_SENSOR_ITEMS;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_ROOT;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_DATA;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.json.JSONObject;

import com.balabas.smarthouse.server.controller.ControllerConstants;
import com.balabas.smarthouse.server.model.EntityClass;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import lombok.experimental.UtilityClass;

@UtilityClass
public class ServerValuesMockUtil {

	public static final String FIRMWARE1 = "FirmWare1";

	public static List<DeviceRequest> getDevicesMock(int count) throws IOException {
		List<DeviceRequest> result = new ArrayList<>();

		for (int i = 0; i < count; i++) {
				result.add(ServerValuesMockUtil.createMockedRequest(i));
		}

		return result;
	}

	public static DeviceRequest createMockedRequest(int i) throws IOException {
		JSONObject data = new JSONObject()
							.put(DEVICE_FIELD_URL_ROOT, "/")
							.put(DEVICE_FIELD_URL_DATA, ControllerConstants.API_V1+ControllerConstants.DEVICES_ROOT+"/mock_deviceId" + i);
		
		ObjectMapper mapper = new ObjectMapper();
	    JsonNode dataJson = mapper.readTree(data.toString());
		
		return DeviceRequest.builder()
			.deviceId("deviceId" + i)
			.deviceFirmware("deviceFirmware" + i)
			.deviceDescr("Mocked device Description" + i)
			.deviceKey("deviceKey" + i)
			.dataJson(dataJson)
			.build();
	}

	public static JSONObject getSettingsSensors(String deviceId, boolean doAlert) {
		return new JSONObject().put(GROUP_SETTINGS, getSettingsJson()).put(GROUP_SENSORS, getSensorsJson(doAlert))
				.put(GROUP_DEVICE, getDeviceJson(deviceId));
	}

	public static JSONObject getSensors(String deviceId, boolean doAlert) {
		return new JSONObject().put(GROUP_SENSORS, getSensorsJson(doAlert)).put(GROUP_DEVICE, getDeviceJson(deviceId));
	}

	private static JSONObject getDeviceJson(String deviceId) {
		return new JSONObject().put(ENTITY_DEVICE_DEVICE_ID, deviceId).put(ENTITY_DEVICE_DEVICE_FIRMWARE, FIRMWARE1);
	}

	private static JSONObject getSensorsJson(boolean doAlertBme) {
		return new JSONObject()
				.put("bh1750", new JSONObject()
						.put(ENTITY_FIELD_DESCRIPTION, "bh1750-descr")
						.put("l", getRandom(10, 100)))
				.put("bme280", new JSONObject()
						.put(ENTITY_FIELD_DESCRIPTION, "bme280-descr")
						.put("p", getRandom(500, 600))
						.put("h", getRandom(0, 100))
						.put("t", doAlertBme?getRandom(20, 30):5))
				.put("DefaultRele", new JSONObject()
						.put(ENTITY_FIELD_DESCRIPTION, "Default relea pin")
						.put(ENTITY_FIELD_ITEM_CLASS, EntityClass.TOGGLE_BUTTON.getItemClass())
						.put(ENTITY_FIELD_ON, getRandomBool())						)
				.put("ds18d20", new JSONObject()
						.put(ENTITY_FIELD_DESCRIPTION, "Temperature DallasTemperature")
						.put("c", 3)
						.put("00000000:t",getRandom(20, 30))
						.put("00000001:t",getRandom(20, 30))
						.put("00000002:t",getRandom(20, 30))
						.put(ENTITY_FIELD_SENSOR_ITEMS, new JSONObject()
								.put("00000000", new JSONObject()
													.put(ENTITY_FIELD_DESCRIPTION, "00000000")
													//.put("t", getRandom(20, 30))
													)
								.put("00000001", new JSONObject()
													.put(ENTITY_FIELD_DESCRIPTION, "00000001")
													//.put("t", getRandom(20, 30))
													)
								.put("00000002", new JSONObject()
													.put(ENTITY_FIELD_DESCRIPTION, "00000002")
													//.put("t", getRandom(20, 30))
													)
								
							)
						);
		
	}

	private static JSONObject getSettingsJson() {
		return new JSONObject().put("WiFiSetting",
				new JSONObject().put("set1", "set1").put("set2", "123").put("set3", "set33"));
	}

	private static int getRandom(int min, int max) {
		if (min > max) {
			int t = max;
			max = min;
			min = t;
		}
		if (min == max) {
			throw new IllegalArgumentException("max must be greater than min");
		}

		Random r = new Random();
		return r.nextInt((max - min) + 1) + min;
	}
	
	private static boolean getRandomBool() {
		Random r =new Random();
		return r.nextBoolean();
	}
}
