package com.balabas.smarthouse.server.device;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import com.balabas.smarthouse.server.DeviceConstants;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.service.DeviceSecurityService;
import com.balabas.smarthouse.server.service.HttpRequestExecutor;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class DeviceFileManager {

	private static final String GET_FILES_URL = "http://%s/dir";
	private static final String POST_FILES_URL = "http://%s/files";
	private static final String DELETE_FILES_URL = "http://%s/files";

	@Autowired
	private DeviceSecurityService securityService;

	@Autowired
	HttpRequestExecutor executor;
	
	@Autowired
	FileUploadService fileUploadService;
	
	@Autowired
	StorageService storage;
	
	public String getFromStorage(String fileName) throws IOException {
		return storage.loadAsString(fileName);
	}

	public List<DeviceFile> getDeviceFiles(IDevice device) {

		if (device != null) {
			if (!device.isVirtualized()) {
				if (device.getIp() != null) {

					String url = preprocessUrl(String.format(GET_FILES_URL, device.getIp()));

					Map<String, String> params = new HashMap<>();
					params.put("swg", "true");

					ResponseEntity<String> resultEntity = executor.executeGetRequest(url, getHeaders(), params);

					if (resultEntity.getStatusCode().equals(HttpStatus.OK)) {
						String resultStr = resultEntity.getBody();

						if (resultStr != null) {
							try {
								JSONObject resultJson = new JSONObject(resultStr);

								JSONObject obj = resultJson.getJSONObject("files");

								JSONArray names = obj.getJSONArray("files");
								JSONArray sizes = obj.getJSONArray("size");

								List<DeviceFile> result = new ArrayList<DeviceFile>();

								for (int i = 0; i < names.length(); i++) {
									String fileName = names.getString(i);
									int fileSize = sizes.getInt(i);

									result.add(new DeviceFile(device.getIp(), fileName, fileSize));
								}

								return result;
							} catch (Exception e) {
								log.error(e);
							}

						}
					}

				} else {
					log.error("Empty data url deviceId=" + device.getName());
				}
			}

			log.error("Empty device");

			return null;
		}

		return Collections.emptyList();
	}

	public boolean postFile(IDevice device, String fileContent, String fileName) {
		try {
			String url = preprocessUrl(String.format(POST_FILES_URL, device.getIp()));

			ResponseEntity<String> response = fileUploadService.postFile(url, fileName, fileContent.getBytes());
			
			if (response.getStatusCode().equals(HttpStatus.OK)) {
				return true;
			}

			return false;
		} catch (Exception e) {
			log.error(e);
		}

		return false;
	}
	
	public boolean deleteFile(IDevice device, String fileName) {
		try {
			String url = preprocessUrl(String.format(DELETE_FILES_URL, device.getIp()));
			
			RestTemplate restTemplate = new RestTemplate();
			Map<String, String> uriVariables = new HashMap<String, String>();
			uriVariables.put("fileName", fileName);
			restTemplate.delete(url, uriVariables );
			
			return true;
		}catch(Exception e) {
			log.error(e);
		}
		
		return false;
	}

	private HttpHeaders getHeaders() {
		HttpHeaders headers = new HttpHeaders();
		headers.set(HttpHeaders.AUTHORIZATION, securityService.getServerKey());
		headers.set(DeviceConstants.HEADER_SREQUEST, DeviceConstants.HEADER_SREQUEST);

		return headers;
	}
	
	private String preprocessUrl(String url) {
		return url.replace("127.0.0.1", "localhost/mock");
	}
}
