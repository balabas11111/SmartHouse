const DATA_MODEL_ROOT_URL = '/info';
const DATA_MODEL_POST_URL = '/info';

const GROUP_SENSORS  = 'sensors';
const GROUP_SETTINGS = 'settings';

const FIELD_ID = "id";
const FIELD_GROUP = "group";
const FIELD_NAME = "name";
const FIELD_DESCR = "descr";

var dataModel={};
var modelToViewHandlers={};

var source = new EventSource('/events');

var localTest = false;

/*--------------------------Menu functions-----------------------------*/
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};


function onLoadPageComplete(){
	httpRequestDisplayStart();
	sendRequest("GET", DATA_MODEL_ROOT_URL, handleRootReceive, "root", 0, 2000);
	initEventSourceChannel();
}

function handleRootReceive(json){
	dataModel = json;
	processGroupToView(GROUP_SETTINGS);
	processGroupToView(GROUP_SENSORS);
}
/*---------------------------Model to view functions----------------------------*/
function processGroupToView(group){
	var entities = dataModel.data[group];
	if(entities!=undefined){
		console.log("Group = ", group);
		/*document.getElementById("mainContent").innerHTML='';*/
		if(entities!=undefined){
			for (var i in entities) {
				processEntityToView(entities[i]);
			}
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
	var handler = getModelToViewHandlerByName(handlerName); 
	
	if(dataModel.data!=undefined && entity.group!=undefined && entity.name!=undefined && key!=undefined){
		dataModel.data[entity.group][entity.name][key]=val;
	}
	
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
		
		if(entity!=undefined){
			if(entity[FIELD_ID]!=undefined && entity[FIELD_GROUP]!=undefined && entity[FIELD_NAME]!=undefined){
				
				var divId=entity[FIELD_GROUP]+'.'+entity[FIELD_NAME]+'.div';
				removeGrayScale(divId);
				
				for (var key in entity) {
				  let val = entity[key];
				  updateEntityViewValue(entity,key,val);
				}
			}
			
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
	
	addGrayScale(div);
	
	if(!validateForm(div,form)){
		return;
	}
	
	var frmComp = document.forms[form];
	var data = new FormData(frmComp);
	
	console.log("form = ", form, " data=",data);
	sendRequest("POST", DATA_MODEL_POST_URL, resultHandler, data, 0, 2000);
}
function handleHttpRequestStatus(success,message){
	console.log("result="+success+" message="+message);
}

function sendRequest(requestmethod, url, handler, data, timeout, errorTimeOut){
	httpRequestDisplayStart();
	console.log("XHR send request data=",data);
	
	var request = new XMLHttpRequest();
	
	request.open(requestmethod, url, true);
	request.timeout=6000;
	request.ontimeout = function(){
		console.log("XHR request timeout "+request.status);
		handleHttpRequestStatus(false,'Ошибка: Превышен интервал ожидания');
		httpRequestDisplayError();
	};
	request.onerror = function(){
		console.log("XHR request error "+request.status);
		handleHttpRequestStatus(false,'Ошибка запроса устройства');
		httpRequestDisplayError();
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
						httpRequestDisplayOk();
					}catch(err){
						ok=false;
						console.log("ERROR = "+err);
						httpRequestDisplayError();
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
						httpRequestDisplayError();
					}
				}
			}else{
				if(this.status!=200){
					console.log('Ошибка: статус='+this.status);
					httpRequestDisplayError();
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
		addSepia(component);
		
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
		
		removeSepia(component);
	}
}
/*-----ShowMessage-----*/
function showMessage(message,clazz){
	console.log('message clazz=',clazz,' message= ',message);
}

/*-------------------------validate functions-----------*/
function markComponentValidityById(id,valid){
	var comp=getComponentById(id);
	
	markComponentValidity(comp,valid);
}

function markComponentValidity(comp,valid){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			if(valid){
				comp.classList.remove('w3-border-red');
			}else{
				comp.classList.add('w3-border-red');
			}
		}
	}
}

function setComponentValidityMessage(comp,message){
	if(comp!=undefined && comp.type!=undefined){
		var type=comp.type.toLowerCase();
		if(type=='input'){
			comp.setCustomValidity(message);
		}
	}
}

function markComponentValidityWithMessage(comp,valid,msg){
	markComponentValidity(comp,valid);
	setComponentValidityMessage(comp,msg);
	
	return msg;
}

function validateForm(div,form){
	markFormAsValid(form);
	return validateFormFunctionDefault(div,form);
}

function markFormAsValid(form){
	var comp = document.getElementById(form);
	var childNodes = comp.querySelectorAll('input');
	
	if(childNodes!=undefined){
		for (var i in childNodes) {
			markComponentValidity(childNodes[i],true);
			/*if(cleanValMsg){
				setComponentValidityMessage(childNodes[i],'');
			}*/
		}
	}
}

function validateFormFunctionDefault(div,formName){
	var form = document.getElementById(formName);
	var validateMessage='';
	
	var isValidForm = form.checkValidity();
	var childNodes=form.querySelectorAll('input');;
	
	if(!isValidForm){
		var labels=form.querySelectorAll('label');
		
		for(var i=0;i<childNodes.length;i++){
			var child=childNodes[i];
			var valid=child.validity.valid;
			
			markComponentValidity(child, valid);
			
			if(!valid){
				validateMessage+=child.validationMessage+' <br>'
			}
		}
	}else{
		for(var i=0;i<childNodes.length;i++){
			markComponentValidity(childNodes[i], true);
		}
	}
	
	if(validateMessage!=undefined && validateMessage.length!=undefined  && validateMessage.length>0){
		showMessage(validateMessage,'error');
		removeGrayScale(div);
		return false;
	}
	
	return true;
}

/*-----------------------------GUI show hide components----------------------------------------*/
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,false);
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,true);
};

function hideShowComponent(componentIdHide,componentIdShow){
	var compHide=document.getElementById(componentIdHide);
	var compShow=document.getElementById(componentIdShow);
	setVisible(compHide,false);
	setVisible(compShow,true);
	
	markFormAsValid(componentIdShow);
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
/*-----grayscale sepia-----*/
function addGrayScale(compId){
	var comp = document.getElementById(compId);
	if(comp!=undefined){
		comp.classList.add("w3-grayscale-max");
	}
}
function removeGrayScale(compId){
	var comp = document.getElementById(compId);
	if(comp!=undefined){
		comp.classList.remove("w3-grayscale-max");
		console.log('Gray removed = ',compId);
	}
}

function addSepia(comp){
	if(comp!=undefined){
		comp.classList.add("w3-sepia-max");
	}
}

function removeSepia(comp){
	if(comp!=undefined){
		comp.classList.remove("w3-sepia-max");
	}
}
/*-----------------------------Http show hide components----------------------------------------*/
function hideShowStatusImages(showImageId){
	hideComponent('httpConnectImg');
	hideComponent('httpOkImg');
	hideComponent('httpErrorImg');
	
	showComponent(showImageId);
}

function httpRequestDisplayStart(){
	hideShowStatusImages('httpConnectImg');
}
function httpRequestDisplayOk(){
	hideShowStatusImages('httpOkImg');
}
function httpRequestDisplayError(){
	hideShowStatusImages('httpErrorImg');
}
