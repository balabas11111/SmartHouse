package com.balabas.smarthouse.server;


import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.PropertySource;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.telegram.telegrambots.ApiContextInitializer;

@SpringBootApplication
@EnableScheduling
@ComponentScan(basePackages = {"com.balabas.smarthouse.server", "com.balabas.smarthouse.telegram"})
@PropertySource(value = "classpath:application.properties", encoding = "UTF-8")
public class ServerApplication {

	private static ConfigurableApplicationContext context;
	
	public static void main(String[] args) {
		ApiContextInitializer.init();
		context = SpringApplication.run(ServerApplication.class, args);
	}
	
    public static void restart() {
        ApplicationArguments args = context.getBean(ApplicationArguments.class);
 
        Thread thread = new Thread(() -> {
            context.close();
            ApiContextInitializer.init();
            context = SpringApplication.run(ServerApplication.class, args.getSourceArgs());
        });
 
        thread.setDaemon(false);
        thread.start();
    }
}
