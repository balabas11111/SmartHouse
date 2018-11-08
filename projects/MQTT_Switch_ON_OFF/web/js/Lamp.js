//--------------------Lamp click handler------------------------------
function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_');
	var url='/'+componentId.substr(0,ind)+'/setValue';
	
	var val=0;
	
	if(componentId.endsWith('0')){val=1;}
	
	updateComponentsByAjaxCall('POST', url, processJsonOnImageComponent,val,0);
}