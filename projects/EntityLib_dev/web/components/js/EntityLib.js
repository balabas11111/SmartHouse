const DATA_MODEL_ROOT_URL = '/info';
const DATA_MODEL_POST_URL = '/info';

var dataModel={};
var modelToViewHandlers={};

var source = new EventSource('/events');

var currentView = 'sensors';
var localTest = false;

/*--------------------------Menu functions-----------------------------*/
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};


function onLoadPageComplete(){
	sendRequest("GET", DATA_MODEL_ROOT_URL, handleRootReceive, "root", 0, 2000);
	initEventSourceChannel();
}

function handleRootReceive(json){
	dataModel = json;
	processGroupToView(currentView);
}
/*---------------------------Model to view functions----------------------------*/
function processGroupToView(group){
	var entities = dataModel.data[group];
	console.log("Group = ", group);
	/*document.getElementById("mainContent").innerHTML='';*/
	if(entities!=undefined){
		for (var i in entities) {
			processEntityToView(entities[i]);
		}
	}
}

function processEntityToView(entity){
	if(entity==undefined || entity.group==undefined || entity.name==undefined || entity.id==undefined){
		return;
	}
	
	var pref = getPreffixForEntity(entity);
	
	console.log('Entity = '+entity.name+'; pref = '+pref);
	
	Object.keys(entity).forEach(function(key) {
		var expId=getEntityViewId(entity,key);
		var val = entity[key];
		
		console.log('key: '+key+', expId: '+expId+', val: '+val);

		updateEntityViewValue(entity,key,val);
	})
}

function getPreffixForEntity(entity){
	return entity.group+'.'+entity.name;
}

function getEntityViewId(entity,key){
	return getPreffixForEntity(entity)+'.'+key;
}
function getEntityEditId(entity,key){
	return getPreffixForEntity(entity)+'.'+key+'.edit';
}
/*---Model to view handlers processing*/
function updateEntityViewValue(entity,key,val){
	var handlerName=getPreffixForEntity(entity);
	var handler = getModelToViewHandlerByName(name); 
		
	handler(entity,key,val);
}

function registerModelToViewHandler(name,handler){
	console.log('Register ModelToView handler ',name);
	modelToViewHandlers[name]=handler;
}

function getModelToViewHandlerByName(name){
	if(name==undefined || modelToViewHandlers[name]==undefined){
		return updateEntityViewValueDefault;
	}

	return modelToViewHandlers[name];
}
/*---Model To View handlers---*/
function updateEntityViewValueDefault(entity,key,val){
	var expId = getEntityViewId(entity,key);
	var expEditId = getEntityEditId(entity,key);
	setComponentValueById(expId,val);
	setComponentValueById(expEditId,val);
}
/*---------------------------Event source functions----------------------------*/
function initEventSourceChannel(){
	if (!!window.EventSource) {
	
	  source.addEventListener('open', function(e) {
	    console.log("Events Connected");
	  }, false);
	
	  source.addEventListener('error', function(e) {
	    if (e.target.readyState != EventSource.OPEN) {
	      console.log("Events Disconnected");
	    }
	  }, false);
	
	  source.addEventListener('message', function(e) {
		  processEventSourceMessage(e.data);
	  }, false);
	
	  source.addEventListener('myevent', function(e) {
	    console.log("myevent", e.data);
	  }, false);
	}
}

function processEventSourceMessage(message){
	console.log("message", message);
	
	if(message=='hello!'){
		return;
	}
	
	try{
		var entity = JSON.parse(message);
		console.log("changed entity = ",entity);
		
		if(entity!=undefined && entity.chgKey!=undefined){
			var key = entity.chgKey;
			var val = entity[key];
			
			updateEntityViewValue(entity,key,val);
		}
	}catch(err){
		console.log("ERROR parse json"+err);
		return;
	}
}
/*---------------------------AJAX functions------------------------------------*/
function isHttpStatusOk(status){
	if(status==200){	return true;	}
	if(localTest && status==0){	return true; }
	
	return false;
}
function formSubmitAsJson(form,div,resultHandler){
	
	var frmComp = document.forms[form];
	var fd = new FormData(frmComp);
	var data = {};

	for (let [key, prop] of fd) {
	  data[key] = prop;
	}

	data = JSON.stringify(data, null, 2);
	
	console.log("form = ", form, " data=",data);
	
	sendRequest("POST", DATA_MODEL_POST_URL, resultHandler, data, 0, 2000);
}
function formSubmitAsForm(form,div,resultHandler){
	
	var frmComp = document.forms[form];
	var data = new FormData(frmComp);
	
	console.log("form = ", form, " data=",data);
	sendRequest("POST", DATA_MODEL_POST_URL, resultHandler, data, 0, 2000);
}
function handleHttpRequestStatus(success,message){
	console.log("result="+success+" message="+message);
}

function sendRequest(requestmethod, url, handler, data, timeout, errorTimeOut){
	console.log("XHR send request data=",data);
	
	var request = new XMLHttpRequest();
	
	request.open(requestmethod, url, true);
	request.timeout=6000;
	request.ontimeout = function(){
		console.log("XHR request timeout "+request.status);
		handleHttpRequestStatus(false,'Ошибка: Превышен интервал ожидания');
	};
	request.onerror = function(){
		console.log("XHR request error "+request.status);
		handleHttpRequestStatus(false,'Ошибка запроса устройства');
	};
	
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
								
				if (isHttpStatusOk(this.status)){
					var ok=true;
					
					try{
						console.log("Response = "+this.responseText);
						var json = JSON.parse(this.responseText);
						if(handler!=undefined){
							handler(json);
						}
						console.log("XHR result = "+this.responseText);
					}catch(err){
						ok=false;
						console.log("ERROR = "+err);
					}
									
					if(timeout>0){
						var reloadTime=timeout;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, data, timeout, errorTimeOut,reloadTime);
					}
					
					console.log("XHR complete = ");
				}else{
					if(errorTimeOut>0){
						var reloadTime=errorTimeOut;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, data, timeout, errorTimeOut, reloadTime);
					}else{
						console.log('Ошибка запроса устройства');
					}
				}
			}else{
				if(this.status!=200){
					console.log('Ошибка: статус='+this.status);
				}
			};
		};
	request.send(data);
};

function addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, body, timeout, errorTimeOut, reloadTime){
	if(reloadTime!='' && reloadTime!=0 && reloadTime>=2000){
		setTimeout(function(){sendRequest(requestmethod, url, handler, body, timeout, errorTimeOut);}, reloadTime);
	}
};

/*----------------------------component values gettters/setters---------------------------*/
function getComponentValueById(id){
	var component = document.getElementById(id);
	return getComponentValue(component);
}

function setComponentValueById(id,val){
	var component = document.getElementById(id);
	setComponentValue(component,val);
}

function getComponentValue(component){
	if(component!=undefined){
		var tagName = component.tagName.toLowerCase();
		
		if (tagName == 'input'){
			if(component.type!=undefined){
				var type=component.type.toLowerCase();
				
				if(type=='checkbox'){
					if(component.hasAttribute("checked") || component.checked){
						return '1';
					}
					return '0';
				}else if(type=='date'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}else if(type=='time'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}else if(type=='datetime-local'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}				
				else{
					return component.value;
				}
			}else{
				return component.value;
			}
		};
		
		if(tagName=='select'){
			return component.selectedIndex;
		}
		
		if (tagName == 'a'){
			return component.href;
		};
		
		if (tagName == 'h1' || tagName == 'h2' || tagName == 'h3' || tagName == 'h4' || tagName == 'h5'
			|| tagName == 'b' || tagName=='label'){
			return component.innerHTML;
		};
	}
	
	return undefined;
}


function setComponentValue(component,val){
	if(component!=undefined){
		var tagName = component.tagName.toLowerCase();
		
		if (tagName =='label' || tagName=='div' || tagName == 'h1' || tagName == 'h2' || tagName == 'h3' || tagName == 'h4' || tagName == 'h5' || tagName == 'h6'
			|| tagName == 'b'){
			component.innerHTML=val;
			return;
		};
		
		if (tagName == 'input'){
			if(component.type!=undefined){
				var type=component.type.toLowerCase();
				
				if(type=='checkbox'){
					var chbVal=false;
					if(val==1 || val=='on' || val=='1' || val=='True' || val=='true'){
						chbVal=true;
						component.checked=chbVal;
					}
					
				}else if(type=='date'){
					const tmp=new Date(val*1000).toISOString().substring(0, 10);
					component.value=tmp;
				}else if(type=='time'){
					const tmp=new Date(val*1000).toISOString().substring(11, 19);
					component.value=tmp;
				}else if(type=='datetime-local'){
					const tmp=new Date(val*1000).toISOString().substring(0, 19);
					component.value=tmp;
				}				
				else{
					component.value=val;
				}
			}else{
				component.value=val;
			}
		};
		
		if(tagName == 'select'){
			component.selectedIndex=val;
		}
		
		if (tagName == 'a'){
			component.href=val;
			showComponent(component.id);
		};
	}
}
/*----------------------------------------------------------------------------*/
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,false);
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,true);
};

function isVisible(comp){
	return (comp!=undefined && comp.style!=undefined && comp.style.display=='block')
}

function setVisible(comp,visible){
	if(comp!=undefined){
		if(visible){
			comp.style.display = "block";
		}else{
			comp.style.display = "none";
		}
	}
}