package com.balabas.telegram.bot.controller;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;


import lombok.extern.log4j.Log4j2;

@CrossOrigin(origins = "http://localhost:4200")
@RestController
@RequestMapping("/api/v1")
@Log4j2
public class BotController {

	@GetMapping("/devices")
	public String hello() {
	    log.info("Get hello");
		return "Hello";
	}
}
