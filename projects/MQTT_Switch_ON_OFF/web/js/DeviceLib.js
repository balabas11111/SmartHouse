//-----------------------------------------------------------------------------------
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	if(comp!=undefined){
		comp.style.display = "none";
	}
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	if(comp!=undefined){
		comp.style.display = "block";
	}
};

function markComponentAs_Valid(comp){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
		}
	}
}
	
function markComponentAs_InValid(comp){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			comp.classList.add('w3-border-red');
		}
	}
}

//-----------------------------------------------------------------

function showMessage(message,className){
	var msgComp=document.getElementById(currentMessageCompName);
	
	if(className!=undefined && className!=''){
		msgComp.setAttribute('class',className);
	}
	
	if(message!=undefined && message!=''){
		msgComp.style.display = "block";
		msgComp.innerHTML = message;
	}else{
		msgComp.style.display = "none"; 
	}
}

//-----------------------------------process Json answers from device----------------
function processSimpleJson(data){
	processSimpleJsonResponse(data,'');	
}

function processSimpleJsonResponse(data,idPreffix){
	var items=data.items;
	for(var i in items){
		var name=items[i].name;
		var val=items[i].val;
		
		if(idPreffix!=undefined && idPreffix!='' && 0!=idPreffix.length){
			name=idPreffix+name;
		}
		
		var component=document.getElementById(name);
		
		if(component!=undefined){
			var tagName = component.tagName.toLowerCase();
			
			if (tagName == 'h1' || tagName == 'h2' || tagName == 'h3' || tagName == 'h4' || tagName == 'h5'){
				component.innerHTML=val;
			};
			
			if (tagName == 'input'){
				if(component.type!=undefined){
					var type=component.type.toLowerCase();
					
					if(type=='checkbox'){
						var chbVal=false;
						if(val==1 || val=='1'){
							chbVal=true;
						}
						component.checked=chbVal;
					}else{
						component.value=val;
					}
				}else{
					component.value=val;
				}
				
				
				
			};
			
			if (tagName == 'a'){
				component.href=val;
			};
		}
	}
}

function processSettingsJson(data){
	var items=data.items;
	var pageHeader='';
	var devId='';
	var devLocation='';
	
	
	for(var i in items){
		var name=items[i].name;
		var val=items[i].val;
		
		var component=document.getElementById(name);
		
		if(name=='DeviceId'){
			devId=val;
		}
		
		if(name=='DeviceLocation'){
			devLocation=val;
		}
		
		if(component!=undefined){
			
			showComponent(component.id);
			var tagName = component.tagName.toLowerCase();
			
			if (tagName == 'h2' || tagName == 'h4'){
				component.innerHTML=val;
			};
			
			if (tagName == 'input'){
				component.value=val;
			};
			
			if (tagName == 'a'){
				component.href=val;
			};
		}
	}
	
	if(devId!=undefined && devId!='' && devLocation!=undefined && devLocation!=''){
		pageHeader='Устройство !!!SmartHouse - '+devId+' '+devLocation;
		document.title=pageHeader;
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






