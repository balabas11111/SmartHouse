package com.balabas.smarthouse.server.security;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.logging.log4j.util.Strings;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.exception.DeviceRequestValidateException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.security.repository.HashedValuesRepository;
import com.balabas.smarthouse.server.security.repository.HashedValuesRepositoryImpl;
import com.balabas.smarthouse.server.util.SecurityUtil;
import com.google.common.hash.Hashing;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceSecurityServiceImpl implements DeviceSecurityService {

	public static final String HEADER_DEVICE = "device";
	
	@Value("${smarthouse.server.key}")
	private String serverKey;
	
	private String serverKeyHash;
	
	@Autowired
	@Qualifier(HashedValuesRepositoryImpl.DISABLED_DEVICE_IDS_REPO)
	private HashedValuesRepository disabledDeviceIds;
	
	private Map<String, DeviceSecurityContext> secContext = new HashMap<>();
	
	@Override
	public void afterPropertiesSet() throws Exception {
		this.serverKeyHash = Hashing.sha1().hashString(serverKey, StandardCharsets.UTF_8).toString();
		log.debug("Key hashed =" + this.serverKeyHash);
	}

	
	@Override
	public String processDeviceRegistrationRequest(DeviceRequest request) throws DeviceRequestValidateException {

		validateDeviceRequestBase(request);
		
		String tempDeviceKey = request.getDeviceKey();
		
		String tempServerKey = SecurityUtil.generateTempServerKey(tempDeviceKey);
		
		String unhashedDeviceToken = 
				SecurityUtil.split(serverKeyHash, tempServerKey, tempDeviceKey);
		
		String unhashedServerToken = 
				SecurityUtil.split(tempServerKey, serverKey, request.getDeviceId());
		
		String deviceHash = SecurityUtil.getHashed(unhashedDeviceToken);
		String serverHash = SecurityUtil.getHashed(unhashedServerToken);
		
		
		log.info("---------------Security service--------------");
		log.info("tempDeviceKey = "+ tempDeviceKey);
		log.info("tempServerKey = "+ tempServerKey);
		log.info("deviceHash = "+ deviceHash);
		log.info("serverHash = "+ serverHash);
		
		secContext.put(request.getDeviceId(), new DeviceSecurityContext(serverHash, deviceHash));
		
		return tempServerKey;
	}

	@Override
	public boolean validateDeviceRequestBase(DeviceRequest request) throws DeviceRequestValidateException {
		if(Strings.isEmpty(request.getDeviceId()) && 
				!request.getHeaders().containsKey(HEADER_DEVICE)) {
			throw new DeviceRequestValidateException(HttpStatus.BAD_REQUEST);
		} 
		
		if(Strings.isEmpty(request.getDeviceId())) {
			request.setDeviceKey(request.getHeaders().get(HEADER_DEVICE).get(0));
		}
		
		if(disabledDeviceIds.containsValue(request.getDeviceId())) {
			throw new DeviceRequestValidateException(HttpStatus.FORBIDDEN);
		}
		
		return true;
	}
	
	@Override
	public boolean validateDeviceRequestBaseSecurity(DeviceRequest request) throws DeviceRequestValidateException {
		boolean noAuthHeaders = 
				request.getHeaders()==null
				|| !request.getHeaders().containsKey(HttpHeaders.AUTHORIZATION)
				|| request.getHeaders().get(HttpHeaders.AUTHORIZATION).isEmpty();
									
		
		if(noAuthHeaders && Strings.isEmpty(request.getDeviceKey())) {
			throw new DeviceRequestValidateException(HttpStatus.UNAUTHORIZED);
		}

		if(Strings.isEmpty(request.getDeviceKey())){
			request.setDeviceKey(request.getHeaders().get(HttpHeaders.AUTHORIZATION).get(0));
		}
		
		return true;
	}

	@Override
	public boolean validateDeviceRequestRegistration(DeviceRequest request) throws DeviceRequestValidateException {
		validateDeviceRequestBase(request);
		validateDeviceRequestBaseSecurity(request);
		
		if(Strings.isEmpty(request.getDataUrl())
				|| Strings.isEmpty(request.getRootUrl())
				|| Strings.isEmpty(request.getDeviceFirmware())) {
			throw new DeviceRequestValidateException(HttpStatus.BAD_REQUEST);
		}
		
		return true;
	}

	@Override
	public boolean validateDeviceRequestDataUpdate(DeviceRequest request) throws DeviceRequestValidateException {
		validateDeviceRequestBase(request);
		validateDeviceRequestBaseSecurity(request);
		
		if(!secContext.containsKey(request.getDeviceId()) 
				|| secContext.get(request.getDeviceId()) == null){
			throw new DeviceRequestValidateException(HttpStatus.UNAUTHORIZED);
		}
		
		String devKeyExpected = secContext.get(request.getDeviceId()).getDeviceKeyHash();
		
		if(devKeyExpected == null) {
			throw new DeviceRequestValidateException(HttpStatus.UNAUTHORIZED);
		}
		
		if(!devKeyExpected.equals(request.getDeviceKey())) {
			List<String> authHeaders = request.getHeaders().getOrDefault(HttpHeaders.AUTHORIZATION, null);
			
			if(authHeaders==null) {
				throw new DeviceRequestValidateException(HttpStatus.UNAUTHORIZED);
			}
			
			if(!authHeaders.stream().anyMatch(h->devKeyExpected.equals(h))) {
				throw new DeviceRequestValidateException(HttpStatus.UNAUTHORIZED);
			}
		}
		
		return true;
	}

	@Override
	public DeviceSecurityContext getDeviceSecurityContext(String deviceId) {
		return secContext.getOrDefault(deviceId, null);
	}


	@Override
	public String getDeviceKey(String deviceId) {
		DeviceSecurityContext cont = getDeviceSecurityContext(deviceId);
		
		return cont==null?null:cont.getDeviceKeyHash();
	}


	@Override
	public String getServerKey(String deviceId) {
		DeviceSecurityContext cont = getDeviceSecurityContext(deviceId);
		
		return cont==null?null:cont.getServerKeyHash();
	}

}
