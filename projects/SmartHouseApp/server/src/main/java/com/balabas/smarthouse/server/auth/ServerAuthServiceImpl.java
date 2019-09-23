package com.balabas.smarthouse.server.auth;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.exception.DeviceOnServerAuthorizationException;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.util.HashUtil;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class ServerAuthServiceImpl implements InitializingBean, ServerAuthService {

	@Value("${smarthouse.server.key}")
	private String key;

	@Override
	public void afterPropertiesSet() throws Exception {
		this.key = HashUtil.getSha1(this.key);
		log.info("Key hashed =" + this.key);
	}

	@Override
	public boolean checkServerKeyHash(String keySha1) {
		boolean result = HashUtil.compareSha1(this.key, keySha1);
		if (!result) {
			log.error("Server key error expected = " + this.key + " actual" + keySha1);
		}
		return result;
	}
	
	@Override
	public boolean checkDeviceRegistrationRequest(DeviceRequest request) throws DeviceOnServerAuthorizationException {
		if(!checkServerKeyHash(request.getServerKey())){
			throw new DeviceOnServerAuthorizationException(request.getDeviceId(), request.getServerKey(), request.getIp());
		}
		return false;
	}

	@Override
	public String getServerKeyHashed() {
		return this.key;
	}

	@Override
	public boolean checkDeviceOnDataUpdatedRequest(DeviceRequest request, Device device)
			throws DeviceOnServerAuthorizationException, ResourceNotFoundException {

		if (device == null) {
			log.error("DeviceId " + request.getDeviceId() + " is not registered");
			throw new ResourceNotFoundException(Device.class, request.getDeviceId());
		}

		if (request.getDeviceKey() == null || !request.getDeviceKey().equals(device.getDeviceKey())) {
			throw new DeviceOnServerAuthorizationException(request.getDeviceId(), request.getDeviceKey(),
					request.getIp(), DeviceOnServerAuthorizationException.MESSAGE_DEVICE_KEY);
		}

		return true;
	}

}
