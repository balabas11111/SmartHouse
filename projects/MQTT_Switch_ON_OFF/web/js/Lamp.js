//-----------------------------Lamps----------------------------------
function processLampJson(data){
	
	var compId_Q=data.name+'_Q';
	var compId_On=data.name+'_On';
	var compId_Off=data.name+'_Off';
	
	var onOff=data.val;
	
	hideComponent(compId_Q);
	
	if(onOff=='1.00' || onOff=='1'){
		hideComponent(compId_Off);
		showComponent(compId_On);
	}else{
		hideComponent(compId_On);
		showComponent(compId_Off);
	}
}
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