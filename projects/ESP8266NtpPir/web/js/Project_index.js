function onLoadPageComplete(){
	w3_close();
	
	registerJsonHandler('bmeMeasurer',processSimpleJson);
	registerJsonHandler('signalLed',processJsonOnImageComponent);
	
	updateComponentsByAjaxCall('GET', '/getJson_settings', processSettingsJson,'',undefined, 0,2000);
	updateComponentsByAjaxCall('GET', '/getJson_providers', processAllJson,'',undefined, 10000,2000);
}

function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_');
	
	var val=0;
	
	if(componentId.endsWith('0')){val=1;};
	var sensor=componentId.substr(0,ind);
	var url='/sdpcv';
	
	updateComponentsByAjaxCall('POST', url, processJsonOnImageComponent,val,sensor,0);
}