package com.balabas.smarthouse.server.service;

import java.io.File;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.DataLine;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import com.google.common.io.Resources;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class SoundPlayer implements ISoundPlayer {

	private static String SOUND_FOLDER = "sounds/";

	@Getter
	@Value("${smarthouse.server.alarm.sound.alarmstarted}")
	private String alarmStartedSound;

	@Getter
	@Value("${smarthouse.server.alarm.sound.alarmfinished:#{null}}")
	private String alarmFinishedSound;

	@Override
	public void playAlarmStarted() {
		log.info("Sound AlarmStarted");
		playSound(SOUND_FOLDER + alarmStartedSound);
	}

	@Override
	public void playAlarmFinished() {
		if (alarmFinishedSound != null) {
			log.info("Sound AlarmFinished ");
			playSound(SOUND_FOLDER + alarmFinishedSound);
		}
	}

	@Override
	public void playSound(String soundName) {
		try {
			String url = Resources.getResource(soundName).getFile();
			File yourFile = new File(url);
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
			log.info("Sound " + soundName);
		} catch (Exception e) {
			log.error(e);
		}
	}

}
