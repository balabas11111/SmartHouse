//--------------------Lamp click handler------------------------------
function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_');
	var url='/'+componentId.substr(0,ind)+'/setValue';
	
	var onOff=componentId.endsWith('Off');
	var val=0;
	
	if(onOff){val=1;}
	
	updateComponentsByAjaxCall('POST', url, processLampJson,val,0);
}

//------------------------------Base logic----------------------------
function onLoadPageComplete(){
	w3_close();
	
	 updateComponentsByAjaxCall('GET', '/espSettingsBox/getJson', processSettingsJson,"", 0);
	
	 updateComponentsByAjaxCall('GET', '/bmeMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/luxMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/dhtMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/ds18d20Measurer/getJson', processDS18D20Json,"", 120000);
	 
	 updateComponentsByAjaxCall('GET', '/lampLeft/getJson', processLampJson,"", 5000);
	 updateComponentsByAjaxCall('GET', '/lampRight/getJson', processLampJson,"", 5000);
	 
	 updateComponentsByAjaxCall('GET', '/pirDetector/getJson', processLampJson,"", 5000);
}