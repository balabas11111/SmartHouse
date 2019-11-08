package com.balabas.smarthouse.server.zzz.mock;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_ROOT;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_URL_DATA;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.json.JSONObject;

import com.balabas.smarthouse.server.controller.ControllerConstants;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

import lombok.experimental.UtilityClass;

@UtilityClass
public class ServerValuesMockUtil {

	public static final String FIRMWARE1 = "FirmWare1";

	public static List<DeviceRequest> getDevicesMockedRegisterRequest(int count) throws IOException {
		List<DeviceRequest> result = new ArrayList<>();

		for (int i = 0; i < count; i++) {
				result.add(ServerValuesMockUtil.createMockedRequest(i));
		}

		return result;
	}

	public static DeviceRequest createMockedRequest(int i) throws IOException {
		JSONObject data = new JSONObject()
							.put(DEVICE_FIELD_URL_ROOT, "/")
							.put(DEVICE_FIELD_URL_DATA, ControllerConstants.MOCK + "/mock_MockedDeviceId" + i);
		
		return DeviceRequest.builder()
			.deviceId("MockedDeviceId" + i)
			.deviceFirmware("deviceFirmware" + i)
			.deviceDescr("Mocked device Description" + i)
			.deviceKey("deviceKey" + i)
			.data(data.toString())
			.build();
	}
	
	public static String getRandomStr(int min, int max) {
		return Integer.toString(getRandom(min, max));
	}

	public static int getRandom(int min, int max) {
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
	
}
