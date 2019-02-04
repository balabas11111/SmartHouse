function onLoadPageComplete(){
	w3_close();
	
	updateComponentsByAjaxCall('GET', '/getJson?name=espSettingsBox', processSettingsJson,'',undefined, 0,2000);
	updateComponentsByAjaxCall('GET', '/getJson?name=all', processAllJson,'',undefined, 10000,2000);
}
