package com.balabas.smarthouse.server.zzz.mock;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import com.balabas.smarthouse.server.controller.ControllerConstants;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.google.common.base.Charsets;
import com.google.common.base.Joiner;
import com.google.common.io.Resources;

import lombok.extern.log4j.Log4j2;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_GROUP;
import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_FIELD_ENTITY_NAME;

import static com.balabas.smarthouse.server.zzz.mock.ServerValuesMockUtil.getRandomStr;

@CrossOrigin(origins = { ControllerConstants.CROSS_ORIGIN_4200, ControllerConstants.CROSS_ORIGIN_80 })
@RestController
@RequestMapping(ControllerConstants.MOCK)
@Log4j2
public class MockedDeviceController {

	@Autowired
	private MockedDeviceService mockService;

	@Autowired
	private IDeviceManageService deviceService;

	boolean doAlert = false;

	boolean doChange = false;
	
	private static List<String> mockedFiles = new ArrayList<String>();
	private static List<Long> mockedSize = new ArrayList<Long>();
	
	private static String getFilesAsJson() {
		initFiles();
		JSONObject root = new JSONObject("{\"files\":{\"files\":[],\"size\":[]}}");
		
		JSONObject files = root.getJSONObject("files");
		
		JSONArray filesa = files.getJSONArray("files");
		JSONArray sizea = files.getJSONArray("size");
		
		for(String mockedFile : mockedFiles) {
			filesa.put(mockedFile);
		}
		
		for(Long mockedSize : mockedSize) {
			sizea.put(mockedSize);
		}
		
		String result = root.toString();
		
		return result;
	}
	
	private static void initFiles() {
		if(mockedFiles.size()==0) {
			addFile("txt/fileName.txt", 123);
		}
	}
	
	private static void addFile(String fileName, long size) {
		mockedFiles.add(fileName);
		mockedSize.add(size);
	}

	@GetMapping("/alert")
	public void doAlert(@RequestParam(value = "doAlert") boolean alert) throws IOException {
		log.info("Mocking doAlert = " + alert);
		doAlert = alert;
	}

	@GetMapping("/change")
	public void noAlertBme(@RequestParam(value = "doChange") boolean change) throws IOException {
		log.info("Mocking doChange = " + change);
		doChange = change;
	}

	@GetMapping("/devices")
	public String changeAlertState() throws IOException {
		log.info("Mocking doChange = " + !doChange);
		doChange = !doChange;

		return "redirect:/manage";
	}
	
	@GetMapping("/dir")
	public ResponseEntity<String> getFiles() {
		String res = getFilesAsJson();
		return ResponseEntity.ok().body(res); 
	}
	
	@PostMapping("/files")
	public ResponseEntity<String> postFiles(
			@RequestParam(name="file", required = false) MultipartFile file,
			@RequestParam(name="uploadfile", required = false) MultipartFile uploadFile,
			@RequestParam(name="filename", required=false) String fileName,
			@ModelAttribute(name="uploadfile") String uploadFileStr,
			@ModelAttribute(name="filename") String fileNameStr) {
		
		if(uploadFileStr!=null) {
			int pos = uploadFileStr.lastIndexOf("=");
			
			addFile(uploadFileStr.substring(pos + 1), 12312);
		}
		
		String res = "ok";
		return ResponseEntity.ok().body(res); 
	}
	
	@DeleteMapping("/files")
	public ResponseEntity<String> deleteFiles(
			@RequestParam(name="fileName", required = false) String file,
			@ModelAttribute(name="fileName") String fileName,
			@ModelAttribute(name="filename") String fileNameStr) {
		
		for(int i = 0; i<mockedFiles.size(); i++ ) {
			if(fileNameStr!=null && fileNameStr.equals(mockedFiles.get(i))) {
				
				mockedFiles.remove(i);
				mockedSize.remove(i);
				break;
			}
		}
		
		String res = "ok";
		return ResponseEntity.ok().body(res); 
	}

	@SuppressWarnings("rawtypes")
	@GetMapping("/mock_{deviceId}")
	public ResponseEntity<String> executeMockGetDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestHeader HttpHeaders headers,
			@RequestParam(value = DEVICE_FIELD_GROUP, required = false) String devEntGroup) throws IOException {

		String pref = "MockedDeviceId";
		String index = deviceId.substring(deviceId.indexOf(pref) + pref.length());

		String resource = (doChange) ? "mock/dataSwgBig.json" : "mock/dataSwg.json";

		URL url = Resources.getResource(resource);
		String result = Resources.toString(url, Charsets.UTF_8).replaceAll("%INDEX%", index);

		if (doAlert) {
			result = result.replaceAll("\"t\": 23", "\"t\": 40");
		} else {
			result = result.replaceAll("\"t\": 23", "\"t\": " + getRandomStr(10, 30));
		}

		result = result.replaceAll("\"h\": 59", "\"h\": " + getRandomStr(20, 100));
		result = result.replaceAll("\"p\": 100553", "\"p\": " + getRandomStr(10020, 100500));

		result = result.replaceAll("\"l\": 1253", "\"l\": " + getRandomStr(50, 60));

		result = result.replaceAll("\"h\": 67", "\"h\": " + getRandomStr(20, 100));
		result = result.replaceAll("\"t\": 28", "\"t\": " + getRandomStr(10, 30));

		result = result.replaceAll("\"0:t\": 36", "\"0:t\": " + getRandomStr(5, 10));
		result = result.replaceAll("\"1:t\": 32", "\"1:t\": " + getRandomStr(5, 10));
		result = result.replaceAll("\"2:t\": 31", "\"2:t\": " + getRandomStr(5, 10));

		boolean isServerRequestValid = mockService.OnDeviceValidateServerKey(headers, deviceId);

		if (!isServerRequestValid) {
			return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body("suck man");
		}

		log.debug("Mock result =" + result);

		JSONObject obj = new JSONObject(result);

		JSONObject deviceObj = obj.getJSONObject("device").getJSONObject("info");
		JSONObject sensorsObj = obj.getJSONObject("sensors");

		String deviceName = deviceObj.getString("deviceId");
/*
		for (IEntityField entityField : deviceService.getEntityFieldsNotVirtualCommandButtons()) {
			EntityFieldBoolean targetEntityField = (EntityFieldBoolean) entityField;

			String targetEntityName = targetEntityField.getEntity().getName();
			String targetDeviceName = targetEntityField.getEntity().getDevice().getName();

			if (deviceName.equals(targetDeviceName) && sensorsObj.has(targetEntityName)) {
				JSONObject entityObj = sensorsObj.getJSONObject(targetEntityName);
				String key = targetEntityField.getName();
				Boolean valB = targetEntityField.getValue();
				boolean value = (valB == null) ? false : valB.booleanValue();
				entityObj.put(key, Boolean.valueOf(value));
				log.debug("Mock entityObj =" + entityObj);
			}
		}
*/
		result = obj.toString();

		return ResponseEntity.ok().body(result);
	}

	@PostMapping("/mock_{deviceId}")
	public ResponseEntity<String> executeMockPostDataOnDevice(@PathVariable(value = "deviceId") String deviceId,
			@RequestHeader HttpHeaders headers, @RequestParam Map<String, String> allRequestParams,
			@RequestBody String body, @RequestParam(value = DEVICE_FIELD_GROUP, required = false) String devEntGroup,
			@RequestParam(value = DEVICE_FIELD_ENTITY_NAME, required = false) String devEntName) throws IOException {

		log.info("POST Mock");
		log.info("Params = " + Joiner.on(",").withKeyValueSeparator("=").join(allRequestParams));

		return executeMockGetDataOnDevice(deviceId, headers, devEntGroup);
	}

}
