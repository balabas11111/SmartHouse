package com.balabas.smarthouse.server.model.request;

import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;
import lombok.Setter;

@Builder
@AllArgsConstructor
public class DeviceRequestResult<T> {

	@Getter @Setter
	private HttpStatus status;
	@Getter
	private HttpHeaders headers;
	@Getter
	private T body;
	
	public static DeviceRequestResult<String> from(HttpStatus status){
		return new DeviceRequestResult<>(status, new HttpHeaders() , status.name());
	}
	
	public static DeviceRequestResult<String> from (HttpStatus status, HttpHeaders headers){
		return new DeviceRequestResult<>(status, headers , "");
	}
	
	public static <T> DeviceRequestResult<T> from (HttpStatus status, T body){
		return new DeviceRequestResult<>(status, new HttpHeaders() , body);
	}
	
	public static <T> DeviceRequestResult<T> from (HttpStatus status, HttpHeaders headers, T body){
		return new DeviceRequestResult<>(status, headers , body);
	}
	
	public ResponseEntity<T> toResponseEntity(){
		return ResponseEntity.status(this.status).headers(this.headers).body(this.body);
	}
}
