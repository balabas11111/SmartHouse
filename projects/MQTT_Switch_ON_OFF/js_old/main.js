//web sockets part------------------------------------------------------------

//----------------------------------form submit unsed currently---------------
function submitForm(vievName,formName,updateName){
	var f = document.forms[formName];
	var isValidForm = f.checkValidity();
	
	if(isValidForm){
		f.addEventListener('submit', function(evt){evt.preventDefault();},false);
		var formData = new FormData(f);
		formData.append("form", formName);
		formData.append("vievName", vievName);
		document.getElementById(updateName).innerHTML = 'Обновляю данные';
		var request = new XMLHttpRequest();
		request.open("POST", "setup", true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					if (this.status == 200){
						document.getElementById(updateName).innerHTML = this.responseText;
						document.getElementById(formName).addEventListener('submit',function(e){e.preventDefault();},false);
						if(postUpdateForm){ postUpdateForm();}
					} else {
						document.getElementById(updateName).innerHTML = this.responseText;
						document.getElementById(formName).addEventListener('submit',function(e){e.preventDefault();},false);
						if(postUpdateForm) postUpdateForm();
					};
				};
			};
		request.send(formData);
	}else{
		document.getElementById('msgContent').innerHTML ='Некоторые значения неверны';
		}
};

function loadForm(viewName,updateName){
	updateHtmlComponentByAjax("vievName",viewName,updateName,"POST","setup","");
};
//----------------------constants---------------------------------------------
//widget actions
const ACTION_GET_STATIC_SETTINGS_DATA="getStSettData";       //returns static data from espSettings
const ACTION_GET_WIDGET_HTML_OR_VAL="getWidgetHtml";       //returns all components text
const ACTION_GET_ALL_WIDGET_JSON="getAllWidgetsJson";
const ACTION_GET_WIDGET_JSON="getWidgetJson";
const ACTION_GET_WIDGETS_CHILDREN_AS_JSON="getWidgetChildrenAsJson";       //returns all components children as json
const ACTION_SUBMIT_FORM_GET_WIDGETS_CHILDREN_AS_JSON="submitFormGetChildrenAsJson"; 
const ACTION_SUBMIT_WIDGET_GET_VALUE="submitAndGetWidget"; //used to process data from client
//url constants
const URL_REMOTE_GET_WIDGETS="handleHttpWidget";       //url to reload httpWidgets on client
const URL_PROCESS_EVENTS="processEvent";       //url to process incoming pin events

const URL_REMOTE_GET_WIDGETS_METHOD="POST";       //method of remote call
//base constants
const PARAM_ACTION_ID="actionName";       //request action, which to be executed on ESP
const PARAM_REMOTE_ID="widgetName";       //request parameter identifies id of entity on ESP board
const PARAM_REMOTE_VAL="remoteVal";       //request parameter which remote value is required
const PARAM_CLASS_NAME="className";       //request parameter identifies class of customers control
const PARAM_CHILD_CLASS="childClassName"; //request parameter identifies child class of customer control to update
const PARAM_CLIENT_DATA="clientData";     //clientData a.e. form in Json format  
//classess
const CLASS_REFRESHABLE_SettingsWidgetESP="SettingsWidgetESP";       //used to auto update espsettings
const CLASS_REFRESHABLE_MeasurerWidgetESP="MeasurerWidgetESP";       //used to update whole html
const CLASS_REFRESHABLE_CHILDREN_MeasurerWidgetESPJson="MeasurerWidgetESPJson"; //used to update form values by json
const CLASS_REFRESHABLE_MeasurerWidgetESPJsonStatus="MeasurerWidgetESPJsonStatus"; //class for display status of ajax
const CLASS_REFRESHABLE_IMAGE="refreshableImage";

const CLASS_REFRESHABLE_CHILD="refreshableChild"; //view of returnable json
//viewNames
//const VIEW_NAME_jsonFormView="jsonFormView"; //view of returnable json
//attributes
const ATTRIBUTE_REMOTE_ID="remoteId";       //remoteId tag. SPecifies widgetName on ESP
const ATTRIBUTE_REMOTE_VAL="remoteVal";     //whichValueSelect from received JSON
const ATTRIBUTE_RELOAD_INTERVAL="reloadInterval";       //used to auto reload some component
const ATTRIBUTE_TARGET_TAG="targetTag";                 //tag which should be updated
const ATTRIBUTE_TARGET_TAG_REPLACE_PART="targetTagReplacePart";  //if partOf tag should be updated 
//id helper constants
const ID_STATUS_SUFFIX="_status";
const STATUS_OK_CLASS="w3-panel w3-green w3-card-4";
const STATUS_ERROR_CLASS="w3-panel w3-red w3-card-4";
//Message constants
const MESSAGE_REFRESH="Обновляю"; 
const MESSAGE_NOT_FOUND="Не найдено"; 
const MESSAGE_ERROR="ОШИБКА"; 
const MESSAGE_REQUEST_ERROR="ОШИБКА запроса"; 
const MESSAGE_REMOTE_ERROR="Удаленная ошибка"; 
const MESSAGE_SERVER_UNAVAILABLE="Устройство недоступно"; 
const MESSAGE_SENSOR_NOT_FOUND="Датчик не найден"; 
const MESSAGE_SENSOR_BROKEN="Датчик неисправен"; 
const MESSAGE_DEVICE_BROKEN="Устройство неисправно"; 

const MIN_TIMEOUT_VALUE_SENSOR=2000;

const WS_TAG_ID="wsId";
const WS_TAG_VALUE="wsValue";
const WS_TAG_PARENT="wsParent";
const WS_TAG_CHILD="wsChild";
const WS_TAG_ITEM="wsItem";

//-------------------base functions----------------------------------
function updateComponentHtmlTagRequiredValueByName(updateName,receivedValue){
	var component=document.getElementById(updateName);
	updateComponentHtmlTagRequiredValue(component,receivedValue);
}

function updateComponentHtmlTagRequiredValue(component,receivedValue){
	
	if(component!=undefined){	
		if(component.hasAttribute(ATTRIBUTE_TARGET_TAG)){
			
			var targetTag=component.getAttribute(ATTRIBUTE_TARGET_TAG);
			var targetTagValue=component.getAttribute(targetTag);
			
			if(component.hasAttribute(ATTRIBUTE_TARGET_TAG_REPLACE_PART)){
				var targetTagReplacePart=component.getAttribute(ATTRIBUTE_TARGET_TAG_REPLACE_PART);
				if(targetTagValue.includes(targetTagReplacePart)){
					targetTagValue=targetTagValue.replace(targetTagReplacePart,receivedValue);
				}
				
			}else{
				targetTagValue=receivedValue;
			}
			
			component.setAttribute(targetTag,targetTagValue);
		}else{
			component.innerHTML = receivedValue;
		}
	}
};

function getWidgetsRemoteIdByName(widgetId){
	var component=document.getElementById(widgetId);
	var remoteId=getWidgetsRemoteId(component);
	
	if(remoteId==undefined){
		remoteId=widgetId;
	}
	
	return remoteId;
};

function getWidgetsRemoteValue(component){
	var remoteVal=undefined;
	
	if(component!=undefined){
		if(component.hasAttribute(ATTRIBUTE_REMOTE_VAL)){
			remoteVal=component.getAttribute(ATTRIBUTE_REMOTE_VAL);
		}else{
			remoteVal="";
		}
	}
	
	return remoteVal;
};

function getWidgetsRemoteId(component){
	var remoteId=undefined;
	
	if(component!=undefined){
		if(component.hasAttribute(ATTRIBUTE_REMOTE_ID)){
			remoteId=component.getAttribute(ATTRIBUTE_REMOTE_ID);
		}else{
			remoteId=component.id;
		}
	}
	
	return remoteId;
};

function returnStatusComponentForWidget(widgetId){
	var statusComponentId=widgetId+ID_STATUS_SUFFIX;
	var componentStatus=document.getElementById(statusComponentId);
	
	return componentStatus;
};

function updateStatusComponentInnerHtml(statusComponent,styleDisplay,className,innerHtml){
	if(statusComponent!=undefined){
		statusComponent.style.display = styleDisplay;
		statusComponent.innerHTML = innerHtml;
		statusComponent.className = className;
		
		return true;
	}
	
	return false;
}
//-------------------update components html------------------------
function reloadAllWidgetsByClassname(actionId, widgetClass, allowAutoRefresh){
	var widgets,i,widgetId,remoteId;
	
	widgets=document.getElementsByClassName(widgetClass);
	
	for (i = 0; i < widgets.length; i++) {
		widgetId=widgets[i].id;
		remoteId=getWidgetsRemoteId(widgets[i]);
	    updateHtmlComponentByAjax(actionId, remoteId, widgetId, widgetClass,"", URL_REMOTE_GET_WIDGETS_METHOD, URL_REMOTE_GET_WIDGETS, allowAutoRefresh);
	}
	
};

function updateHtmlComponentByAjax(actionId, remoteId, widgetId, widgetClass, clientData, requestmethod, url, allowAutoRefresh){
	var component=document.getElementById(widgetId);
	var remoteVal=getWidgetsRemoteValue(component)
	
	//component.innerHTML = MESSAGE_REFRESH;
	
	var formData = new FormData();
	formData.append(PARAM_ACTION_ID, actionId);
	formData.append(PARAM_REMOTE_ID, remoteId);
	formData.append(PARAM_REMOTE_VAL, remoteVal);
	formData.append(PARAM_CLASS_NAME, widgetClass);
	formData.append(PARAM_CHILD_CLASS, "");
	formData.append(PARAM_CLIENT_DATA, clientData);
	var request = new XMLHttpRequest();
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
				if (this.status == 200){
					
					updateComponentHtmlTagRequiredValue(component,this.responseText);
					
					if(allowAutoRefresh==true && component.hasAttribute(ATTRIBUTE_RELOAD_INTERVAL)==true){
						var timeout = component.getAttribute(ATTRIBUTE_RELOAD_INTERVAL);
						addPostponedUpdate(actionId, remoteId, widgetId, widgetClass, clientData, requestmethod, url, allowAutoRefresh, timeout);
					}
				}else{
					addPostponedUpdate(actionId, remoteId, widgetId, widgetClass, clientData, requestmethod, url, allowAutoRefresh, timeout);
					//component.innerHTML = this.responseText;
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdate(actionId, remoteId, widgetId, widgetClass, clientData, requestmethod, url, allowAutoRefresh, timeout){
	if(timeout!='' && timeout!=0 && timeout>=MIN_TIMEOUT_VALUE_SENSOR){
		setTimeout(function(){updateHtmlComponentByAjax(actionId, remoteId, widgetId, widgetClass, clientData, requestmethod, url, allowAutoRefresh);}, timeout);
	}
};

//-------------------update components childrens values------------------------

function reloadAllWidgetsChildsByClassnameJson(actionId, widgetClass, childClass, allowAutoRefresh){
	var widgets,children,i,container,widgetId,remoteId;
	
	widgets=document.getElementsByClassName(widgetClass);
	
	for (i = 0; i < widgets.length; i++) {
		container=widgets[i];
		widgetId=container.id;
		remoteId=getWidgetsRemoteId(container);
		
		children=container.getElementsByClassName(childClass);
	    updateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass,"", URL_REMOTE_GET_WIDGETS_METHOD, URL_REMOTE_GET_WIDGETS, allowAutoRefresh);
	}
};

function updateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass, clientData, requestmethod, url, allowAutoRefresh){
	
	var statusComponent=returnStatusComponentForWidget(widgetId);
	
	updateStatusComponentInnerHtml(statusComponent,"block",STATUS_OK_CLASS,MESSAGE_REFRESH);
		
	var formData = new FormData();
	formData.append(PARAM_ACTION_ID, actionId);
	formData.append(PARAM_REMOTE_ID, remoteId);
	formData.append(PARAM_CLASS_NAME, widgetClass);
	formData.append(PARAM_CHILD_CLASS, childClass);
	formData.append(PARAM_CLIENT_DATA, clientData);
	
	var request = new XMLHttpRequest();
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
				var container=document.getElementById(widgetId);
				
				if (this.status == 200){
					//parse container controls
						var children, index, componentTag, childComponent,componentId,receivedValue;
						
							children = container.getElementsByClassName(childClass);
							var json = JSON.parse(this.responseText);
							
							for (index = 0; index < children.length; ++index) {
								childComponent=children[index];
								componentTag=childComponent.tagName.toLowerCase();
																
								componentId=getWidgetsRemoteValue(childComponent);								
																
								if(json.hasOwnProperty(componentId)){
									receivedValue=json[componentId];
									updateComponentHtmlTagRequiredValue(childComponent,receivedValue);
								}
								
							}
					//finishParse controls
				
					if(allowAutoRefresh==true && container.hasAttribute(ATTRIBUTE_RELOAD_INTERVAL)==true){
						var timeout = container.getAttribute(ATTRIBUTE_RELOAD_INTERVAL);
						addPostponedUpdateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass, clientData, requestmethod, url, allowAutoRefresh, timeout);
					}
					
					updateStatusComponentInnerHtml(statusComponent,"none",STATUS_OK_CLASS,MESSAGE_REFRESH);
					
				}else{
					//
					addPostponedUpdateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass, clientData, requestmethod, url, allowAutoRefresh, 5000);
					/*
					if(!updateStatusComponentInnerHtml(statusComponent,"block",STATUS_ERROR_CLASS,MESSAGE_ERROR)){
						container.innerHTML = MESSAGE_ERROR+" "+this.responseText;
					}*/
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass, clientData, requestmethod, url, allowAutoRefresh, timeout){
	if(timeout!='' && timeout!=0 && timeout>MIN_TIMEOUT_VALUE_SENSOR){
		setTimeout(function(){updateComponentsChildrenByAjaxJson(actionId, remoteId, widgetId, widgetClass, childClass, clientData, requestmethod, url, allowAutoRefresh);}, timeout);
	}
};
//--------------------------Menu functions-----------------------------
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};

function printMainStatus(message,compClass){
	var widgetId="websocket_status";
	var component=document.getElementById(widgetId);
		
	updateStatusComponentInnerHtml(component,"block",compClass,message);
}
//--------------------Lamp click handler------------------------------
function lampWidgetClick(component){
	remoteId=getWidgetsRemoteId(component);
	widgetId=component.id;
	widgetClass=CLASS_REFRESHABLE_IMAGE;
	clientData=component.src;
	updateHtmlComponentByAjax(ACTION_SUBMIT_WIDGET_GET_VALUE, remoteId, widgetId, widgetClass ,clientData, URL_REMOTE_GET_WIDGETS_METHOD, URL_REMOTE_GET_WIDGETS, false);
}

//---------------------On Load functions-------------------------------
function onLoadPageComplete(){
	
	var f= document.forms["publish_form"];
	
	if(f!=undefined){
		f.addEventListener('submit', function(evt){evt.preventDefault();},false);
	}
	
	w3_close();
	//WebSocketInit();
	reloadAllWidgetsByClassname(ACTION_GET_STATIC_SETTINGS_DATA,CLASS_REFRESHABLE_SettingsWidgetESP,false);
	//reloadAllWidgetsByClassname(ACTION_GET_WIDGET_HTML_OR_VAL,CLASS_REFRESHABLE_MeasurerWidgetESP,true);
	reloadAllWidgetsByClassname(ACTION_GET_WIDGET_HTML_OR_VAL,CLASS_REFRESHABLE_IMAGE,true);
	reloadAllWidgetsChildsByClassnameJson(ACTION_GET_WIDGETS_CHILDREN_AS_JSON,CLASS_REFRESHABLE_CHILDREN_MeasurerWidgetESPJson, CLASS_REFRESHABLE_CHILD,true);
};



