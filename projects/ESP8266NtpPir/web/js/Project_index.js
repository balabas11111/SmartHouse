function onLoadPageComplete(){
	w3_close();
	
	updateComponentsByAjaxCall('GET', '/espSettingsBox/getSimpleJson', processSettingsJson,'',undefined, 0,2000);
	updateComponentsByAjaxCall('GET', '/gscv?sensor=all', processAllJson,'',undefined, 10000,2000);
}

var handlersMap=new Map(handlersMapGenerator);

var handlersMapGenerator=function*(){
	yield ['bmeMeasurer',processSimpleJson];
	yield ['luxMeasurer',processSimpleJson];
	yield ['ds18d20Measurer',processSimpleJsonRecreateChildren];
	yield ['lampLeft',processJsonOnImageComponent];
	yield ['lampRight',processJsonOnImageComponent];
	yield ['acMeter',processJsonOnImageComponent];
};

function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_');
	
	var val=0;
	
	if(componentId.endsWith('0')){val=1;};
	var sensor=componentId.substr(0,ind);
	var url='/sdpcv';
	
	updateComponentsByAjaxCall('POST', url, processJsonOnImageComponent,val,sensor,0);
}