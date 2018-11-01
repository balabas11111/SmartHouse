function hideComponent(componentId){
	document.getElementById(componentId).style.display = "none";
}

function showComponent(componentId){
	document.getElementById(componentId).style.display = "block";
}

var processLampJsonF=function processLampJson(data){
	
	var compId_Q=data.name+'_Q';
	var compId_On=data.name+'_On';
	var compId_Off=data.name+'_Off';
	
	var onOff=data.val;
	
	hideComponent(compId_Q);
	
	if(0nOff==1.00){
		hideComponent(compId_Off);
		showComponent(compId_On);
	}else{
		hideComponent(compId_On);
		showComponent(compId_Off);
	}
}

var processMeasurerJsonF=function processMeasurerJson(data){
	var items=data.items;
	for(var i in items){
		var name=items[i].name;
		var val=items[i].val;
		
		document.getElementById(name).value=val;
	}
}

function updateComponentsByAjaxCall(requestmethod, url, handler, val, timeout){
	
	var request = new XMLHttpRequest();
	
	var formData = new FormData();
	formData.append('val', val);
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
								
				if (this.status == 200){
					
					var json = JSON.parse(this.responseText);
							
					handler(json);
									
					if(timeout>0){
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);
					}
					
				}else{
					//
					addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout){
	if(timeout!='' && timeout!=0 && timeout>2000){
		setTimeout(function(){updateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);}, timeout);
	}
};

//--------------------Lamp click handler------------------------------
function lampWidgetClick(component){
	var componentId=component.id;
	var ind=componentId.indexOf('_')-1;
	var url='/'+componentId.substr(0,ind)+'/setValue';
	
	updateComponentsByAjaxCall('POST', url, processLampJsonF,componentId.endsWith('Off'),0);
}

function onLoadPageComplete(){
	 updateComponentsByAjaxCall('GET', '/bmeMeasurer/getSimpleJson', processMeasurerJsonF,"", 120000);
	 updateComponentsByAjaxCall('GET', '/luxMeasurer/getSimpleJson', processMeasurerJsonF,"", 120000);
	 
	 updateComponentsByAjaxCall('GET', '/lampLeft/getSimpleJson', processLampJsonF,"", 5000);
	 updateComponentsByAjaxCall('GET', '/lampRight/getSimpleJson', processLampJsonF,"", 5000);
}


//--------------------------Menu functions-----------------------------
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};

