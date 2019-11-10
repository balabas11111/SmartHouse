package com.balabas.smarthouse.server.service;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.DataLine;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class SoundPlayer implements ISoundPlayer {

	@Getter
	@Value("${smarthouse.server.alarm.sound.enabled:true}")
	private boolean soundEnabled;

	@Getter
	@Value("${smarthouse.server.alarm.sound.usebeeper.start:true}")
	private boolean useBeeperStart;
	
	@Getter
	@Value("${smarthouse.server.alarm.sound.usebeeper.finish:false}")
	private boolean useBeeperFinish;

	@Getter
	@Value("${smarthouse.server.alarm.sound.beepinterval:50}")
	private int beepInterval;

	@Getter
	@Value("${smarthouse.server.alarm.sound.beepcount:3}")
	private int beepCount;

	@Getter
	@Value("${smarthouse.server.alarm.sound.alarmstarted}")
	private String alarmStartedSound;

	@Getter
	@Value("${smarthouse.server.alarm.sound.alarmfinished:#{null}}")
	private String alarmFinishedSound;

	@Override
	public void playAlarmStarted() {
		if (useBeeperStart) {
			doBeep();
		} else {
			log.info("Sound AlarmStarted");
			playSound(alarmStartedSound);
		}
	}

	@Override
	public void playAlarmFinished() {
		if (useBeeperFinish) {
			doBeep();
		} else {
			if (alarmFinishedSound != null) {
				log.info("Sound AlarmFinished ");
				playSound(alarmFinishedSound);
			}
		}
	}

	@Override
	public void playSound(String filePath) {
		if (soundEnabled) {
			try {
				File yourFile = new File(filePath);
				AudioInputStream stream;
				AudioFormat format;
				DataLine.Info info;
				Clip clip;

				stream = AudioSystem.getAudioInputStream(yourFile);
				format = stream.getFormat();
				info = new DataLine.Info(Clip.class, format);
				clip = (Clip) AudioSystem.getLine(info);
				clip.open(stream);
				clip.start();
				log.info("Sound " + filePath);
			} catch (Exception e) {
				log.error(e);
			}
		}
	}

	@Override
	public void doBeep() {
		if (soundEnabled) {
			ExecutorService executorService = Executors.newSingleThreadExecutor();
			executorService.submit(new BeepThread(beepCount, beepInterval));
			executorService.shutdown();
		}
	}

}
