package com.balabas.smarthouse.server.service;

import com.pi4j.io.gpio.GpioController;
import com.pi4j.io.gpio.GpioFactory;
import com.pi4j.io.gpio.GpioPinDigitalOutput;
import com.pi4j.io.gpio.PinState;
import com.pi4j.io.gpio.RaspiPin;
import com.pi4j.wiringpi.GpioUtil;

import lombok.extern.log4j.Log4j2;

@Log4j2
public class BeepThread implements Runnable {

	private GpioPinDigitalOutput pin;
	private int beepCount = 3;
	private int interval = 3;

	public BeepThread(int beepCount, int interval) {
		super();
		this.beepCount = beepCount;
		this.interval = interval;
	}

	@Override
	public void run() {
		GpioController gpio = null;
		try {
		gpio = GpioFactory.getInstance();
		
		pin = gpio.provisionDigitalOutputPin(RaspiPin.GPIO_18, "MyLED", PinState.LOW);
		pin.setShutdownOptions(true, PinState.LOW);
		
		for (int i = 0; i < beepCount; i++) {
			try {
				doBeep(GpioUtil.DIRECTION_HIGH);
				doBeep(GpioUtil.DIRECTION_LOW);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

			log.info("beeped");
		}catch(Exception e) {
			log.error(e);
		}finally {
			if(gpio!=null && pin!=null) {
				gpio.shutdown();
				gpio.unprovisionPin(pin);
			}
		}
	}

	private void doBeep(int direction) throws InterruptedException {
		GpioUtil.setDirection(pin.getPin().getAddress(), direction);
		Thread.sleep(interval);
	}
}
