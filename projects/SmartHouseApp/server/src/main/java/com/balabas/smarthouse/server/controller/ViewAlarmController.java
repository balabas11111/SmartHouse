package com.balabas.smarthouse.server.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2Service;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IAlarmV2Service alarmService;

	@GetMapping("/alarmsList")
	public String getAllAlarms(Model model) {
		model.addAttribute("serverName", serverName);

		model.addAttribute("alarms", alarmService.getAlarmsGrouppedByItemClassName());

		return "alarmsList.html";
	}

	

}
