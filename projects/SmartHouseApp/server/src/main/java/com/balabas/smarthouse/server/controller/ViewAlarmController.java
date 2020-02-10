package com.balabas.smarthouse.server.controller;

import static com.balabas.smarthouse.server.controller.ControllerConstants.ATTR_SERVER_NAME;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;

import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2Service;

@Controller
public class ViewAlarmController {

	@Value("${smarthouse.server.name:#{null}}")
	private String serverName;

	@Autowired
	private IAlarmV2Service alarmService;

	@GetMapping("/alarmsList")
	public String getAllAlarms(Model model) {
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarms", alarmService.getAlarmsGrouppedByItemClassName());

		return "alarms/alarmsList.html";
	}

	@GetMapping(value = "/editAlarm")
	public String editAlarm(@RequestParam(name = "id", required = true) Long id, Model model) {
		model.addAttribute(ATTR_SERVER_NAME, serverName);
		model.addAttribute("alarm", alarmService.getAlarm(id));
		
		return "alarms/editAlarm.html";
	}
	
	@GetMapping("/deleteAlarm_{id}")
	public String deleteEntityFieldEnabledValue(@PathVariable(name = "id", required = true) Long id, Model model) {
		alarmService.deleteAlarm(id);
		
		return "redirect:/alarmsList";
	}

}
