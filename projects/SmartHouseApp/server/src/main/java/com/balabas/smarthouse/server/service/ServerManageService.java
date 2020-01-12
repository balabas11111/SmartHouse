package com.balabas.smarthouse.server.service;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.ServerApplication;

import lombok.extern.log4j.Log4j2;

@Service
@Log4j2
public class ServerManageService implements IServerManageService {

	@Override
	public void restartServer(int delaySeconds) {
		log.info("server restart in sec = " + delaySeconds);
		ScheduledExecutorService executorService = Executors.newSingleThreadScheduledExecutor();
		
		Runnable restartTask = new Runnable() {
			@Override
			public void run() {
				ServerApplication.restart();
			}
		};
		
		executorService.schedule(restartTask, delaySeconds, TimeUnit.SECONDS);
	}

}
