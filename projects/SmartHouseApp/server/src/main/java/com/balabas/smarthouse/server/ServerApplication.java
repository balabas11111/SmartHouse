package com.balabas.smarthouse.server;


import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.telegram.telegrambots.ApiContextInitializer;

@SpringBootApplication
@EnableScheduling
@ComponentScan(basePackages = {"com.balabas.smarthouse.server", "com.balabas.smarthouse.telegram"})
public class ServerApplication {

	public static void main(String[] args) {
		ApiContextInitializer.init();
		SpringApplication.run(ServerApplication.class, args);
	}
	
}
