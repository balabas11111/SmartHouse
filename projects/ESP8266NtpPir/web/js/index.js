function onLoadPageComplete(){
	w3_close();
	
	 updateComponentsByAjaxCall('GET', '/espSettingsBox/getSimpleJson', processSettingsJson,"",undefined, 0,2000);
	
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=all', processAllJson,"",undefined, 10000,2000);
	 
	 /*
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=bmeMeasurer', processSimpleJson,"",undefined, 120000);
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=luxMeasurer', processSimpleJson,"",undefined, 120000);
	 updateComponentsByAjaxCall('GET', '/dhtMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=ds18d20Measurer', processSimpleJsonRecreateChildren,"",undefined, 120000);
	 
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=lampLeft', processJsonOnImageComponent,"",undefined, 15000);
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=lampRight', processJsonOnImageComponent,"",undefined, 15000);
	 
	 updateComponentsByAjaxCall('GET', '/gscv?sensor=acMeter', processJsonOnImageComponent,"",undefined, 5000);
	 updateComponentsByAjaxCall('GET', '/pirDetector/getJson', processLampJson,"", 5000);
	 */
}

var handlersMapGenerator=function*(){
	yield ['bmeMeasurer',processSimpleJson];
	yield ['luxMeasurer',processSimpleJson];
	yield ['ds18d20Measurer',processSimpleJsonRecreateChildren];
	yield ['lampLeft',processJsonOnImageComponent];
	yield ['lampRight',processJsonOnImageComponent];
	yield ['acMeter',processJsonOnImageComponent];
}

/*--------------------Lamp click handler------------------------------*/
function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_');
	
	var val=0;
	
	if(componentId.endsWith('0')){val=1;}
	var sensor=componentId.substr(0,ind);
	var url='/sdpcv';
	
	updateComponentsByAjaxCall('POST', url, processJsonOnImageComponent,val,sensor,0);
}