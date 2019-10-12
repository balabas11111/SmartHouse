package com.balabas.smarthouse.server.entity.model;

public enum EntityState {

	NONE,
	REGISTERED,
	OK,
	DATA_SEND,
	DATA_RECEIVE,
	WAITS_DATA_SEND,
	WAITS_DATA_RECEIVE,
	DISCONNECTED,
	RECONNECTING,
	FAILED,
	TIMED_OUT
}
