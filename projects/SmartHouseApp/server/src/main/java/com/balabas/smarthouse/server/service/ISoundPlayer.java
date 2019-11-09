package com.balabas.smarthouse.server.service;

public interface ISoundPlayer {

	void playAlarmStarted();
	void playAlarmFinished();
	
	void playSound(String filePath);
}
