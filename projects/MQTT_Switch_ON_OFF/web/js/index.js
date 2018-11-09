

//------------------------------Base logic----------------------------
function onLoadPageComplete(){
	w3_close();
	
	 updateComponentsByAjaxCall('GET', '/espSettingsBox/getSimpleJson', processSettingsJson,"", 0);
	
	 updateComponentsByAjaxCall('GET', '/bmeMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/luxMeasurer/getJson', processSimpleJson,"", 120000);
	 //updateComponentsByAjaxCall('GET', '/dhtMeasurer/getJson', processSimpleJson,"", 120000);
	 updateComponentsByAjaxCall('GET', '/ds18d20Measurer/getJson', processSimpleJsonRecreateChildren,"", 120000);
	 
	 updateComponentsByAjaxCall('GET', '/lampLeft/getJson', processJsonOnImageComponent,"", 15000);
	 updateComponentsByAjaxCall('GET', '/lampRight/getJson', processJsonOnImageComponent,"", 15000);
	 
	 //updateComponentsByAjaxCall('GET', '/pirDetector/getJson', processLampJson,"", 5000);
}