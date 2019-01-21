/*----------------------------------------------------------------------*/
const FIELD_DESCR_ID   = 1;
const FIELD_MIN_ID     = 2;
const FIELD_MAX_ID     = 3;
const FIELD_FIELDID_ID = 4;
const FIELD_QUEUE_ID   = 5;
const FIELD_NAME_ID   = 6;

const SENSOR_NAME_ATTR='sensorname';
const ITEM_NAME_ATTR='itemname';

const SENSORS="sensors_";
const SENSORS_SHORT="s_";

var currentTab='';
var currentHeaderName='';

var currentForm=undefined;
var currentFormList=undefined;

var getValuesHandler=undefined;
var validateValuesHandler=undefined;

var putItemsToContainerHandler=undefined;
var currentItemPreffix=undefined;
var itemsTagName=undefined;

var getValuesUrl='';
var submitValuesUrl='';

var currentJson='';

function openTabGui(tabName,headerName){
	customOnGetMessage=undefined;
	openTab(tabName,headerName);
}

function openTab(tabName,headerName) {
	restartFunctionHandler=reloadCurrentSettingsTab;
	var loadMessagePreffix="Загружаю";
	
	if(currentTab==tabName){
		loadMessagePreffix="Обновляю";
	}
	
	currentTab=tabName;
	currentHeaderName=headerName;
	
	var headerComponent=document.getElementById('settingsHeaderName');
	
	if(headerComponent!=undefined){
		headerComponent.innerHTML=currentHeaderName;
	}
	
    var i;
    var x = document.getElementsByClassName("settingsTab");
    for (i = 0; i < x.length; i++) {
       x[i].style.display = "none";  
    }
    document.getElementById(tabName).style.display = "block"; 

	var containerComponent=document.getElementById(tabName);
	
	setStatusMessageComp(getComponentById(getComponentIdWithSuffix(tabName,MSG_SUFFIX)));
	
	var currentFormId=getComponentIdWithSuffix(tabName,FORM_SUFFIX);
	
	document.title='Настройки устройства - '+currentHeaderName;
	
	if(containerComponent.classList.contains('reloadableSettingsContainer')){
				
		
		showStatusMessage(loadMessagePreffix+' настройки...','w3-yellow');
		
		var handler=undefined;
		
		if(tabName=='device'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateDeviceSettingsForm;
			submitValuesUrl='/processJson?name=espSettingsBox&page=device';
			getValuesUrl='/getJson?name=espSettingsBox&page=device';
		}
		
		if(tabName=='net'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/processJson?name=espSettingsBox&page=net';
			getValuesUrl='/getJson?name=espSettingsBox&page=net';
		}
		
		if(tabName=='publish'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/processJson?name=espSettingsBox&page=publish';
			getValuesUrl='/getJson?name=espSettingsBox&page=publish';
		}
		
		if(tabName=='sensors'){
			getValuesHandler=processItemsJsonGet;
			putItemsToContainerHandler=putSensorContentToContainer;
			currentItemPreffix='currentSensor';
			itemsTagName='sensors';
			validateValuesHandler=validateCurrentSensorForm;
			submitValuesUrl='/processJson?name=deviceHelper&page=sensors';
			getValuesUrl='/getJson?name=deviceHelper&page=sensors';
			currentFormId=getComponentIdWithSuffix(currentItemPreffix,FORM_SUFFIX);
		}
		
		if(tabName=='intervals'){
			getValuesHandler=processIntervalsJsonGet;
			putItemsToContainerHandler=putIntervalContentToContainer;
			currentItemPreffix='currentInterval';
			validateValuesHandler=validateCurrentIntervalForm;
			submitValuesUrl='/processJson?name=timeIntervalService';
			getValuesUrl='/getJson?name=timeIntervalService';
			currentFormId=getComponentIdWithSuffix(currentItemPreffix,FORM_SUFFIX);
		}
		
		currentFormList=getComponentById(getComponentIdWithSuffix(tabName,FORM_SUFFIX));
		currentForm=getComponentById(currentFormId);

		if(getValuesHandler!=undefined){
			currentFormList.addEventListener('submit', 
									function(evt){
										evt.preventDefault();
									},false);
			currentForm.addEventListener('submit', 
								function(evt){
									evt.preventDefault();
								},false);
			
			getValuesUrl=updateUrlEnvironment(getValuesUrl);
			submitValuesUrl=updateUrlEnvironment(submitValuesUrl);
			
			updateComponentsByAjaxCall('GET', getValuesUrl, getValuesHandler,"",undefined, 0,2000);
		}else{
			alert('Error on page!');
		}
	}
}

function updateUrlEnvironment(url){
	if(localTest && url.substring(0,1)=='/'){
		url=url.substring(1,url.length)+'.json';
		url=url.replace('?', '');
	}
	return url;
}

function reloadCurrentSettingsTab(){
	openTab(currentTab,currentHeaderName);
}

/*---------------------------------device settings tab----------------------*/
function processDeviceSettingsGet(data){
	processSimpleJsonResponse(data,'');
	showStatusMessage('Загружено '+currentHeaderName,'w3-green');
}

function processDeviceSettingsSave(data){
	processSimpleJsonResponse(data.data,'');
	showStatusMessage('Сохранено и загружено '+currentHeaderName,'w3-green');
	openCommandRestartPopup();
}

function validateDeviceSettingsForm(){
	var errorMessage=validateFormFunctionDefault(currentForm);
	
	var pass=document.getElementById('espSettingsBox_accessPass');
	var conf=document.getElementById('espSettingsBox_accessPassConfirm');
	
	errorMessage+=processPassConfirmPath(pass,conf);
	
	pass=document.getElementById('espSettingsBox_settingsPass');
	conf=document.getElementById('espSettingsBox_settingsPass');
	
	errorMessage+=processPassConfirmPath(pass,conf);
	
	if(!pass || pass.value==undefined || 0===pass.length){
		errorMessage+='Доступ на страницу пароля возможен только по паролю!';
	}
			
	return errorMessage;
}

function processPassConfirmPath(pass,conf){
	var message="";
	
	markComponentValidity(pass,true);
	markComponentValidity(conf,true);
	
	if(pass.value!=conf.value){
		markComponentValidity(pass,false);
		markComponentValidity(conf,false);
		
		message="Пароль и подьверждение пароля не равны";
	}
	
	return message;
}

function ValidateIPaddress(ipaddress) {  
  if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(ipaddress)) {  
    return (true);  
  }  
  alert("You have entered an invalid IP address!");  
  return (false)  
}

var customOnGetMessage=undefined;

function processItemsJsonGet(data){
	var container=currentFormList;

	container.innerHTML='';
	var items=data[itemsTagName];
	
	currentJson=items;
			
	for(var s in items){
		var item=items[s];
		
		putItemsToContainerHandler(container,item,false,false);
	}
	
	var msg=(customOnGetMessage!=undefined)?customOnGetMessage:('Загружено '+currentHeaderName);
	
	showStatusMessage(msg,'w3-green');
	customOnGetMessage=undefined;
}

function getCurrentItemFromCurrentJson(tagName,tagValue){
	for(var i in currentJson){
		var item=currentJson[i];
		if(item[tagName]==tagValue){
			return currentJson[i];
		}
	}
	
	return undefined;
}

function getCurrentItemFromParentItem(parentItem,parentItemTag,tagName,tagValue){
	var items=parentItem[parentItemTag];
	for(var i in items){
		var tmp=items[i];
		if(tmp[tagName]==tagValue){
			return items[i];
		}
	}
	
	return undefined;
}

function openItemPopup(tagName,sensorName,headerValue,itemC){
	var item;
	
	if(itemC!=undefined){
		item=itemC;
	}else if(sensorName!=undefined){
		item=getCurrentItemFromCurrentJson(tagName,sensorName);
	}
	
	setStatusMessageComp(getComponentById(currentItemPreffix+'_msg'));
	
	if(item!=undefined){
		var sensorNameContainer=getComponentById(currentItemPreffix+'_name');
		var sensorNameHeader=getComponentById(currentItemPreffix+'_header');
		var container=getComponentById(currentItemPreffix+'_content');
		
		sensorNameContainer.value=sensorName;
		sensorNameHeader.innerHTML=headerValue;
		container.innerHTML='';
		
		putItemsToContainerHandler(container,item,true,true);
		hideComponent(currentItemPreffix+'_msg');
		showComponent(currentItemPreffix+'_modal');
	}
}

function getCurrentItemHandler(data){
	if(isStatusMessageResponseOk(data.statusHttp)){
		hideComponent(currentItemPreffix+'_modal');
		reloadCurrentSettingsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}
/*--------------------------GUI creation part-----------------------------*/
function createItemDeleteButton(tagName,tagValue,itemDescr,handler){
	var div=document.createElement('div');
		div.setAttribute('class','w3-tooltip');
		div.setAttribute('style','margin-top: 10px; margin-left: 3px; width: 30px');
	
	var text='&#9986 Удалить';
	if(itemDescr!=undefined){
		text+=' ';
		text+=itemDescr;
	}
	
	var span=document.createElement('span');
		span.setAttribute('class','w3-text w3-tag w3-gray w3-round');
		span.setAttribute('style','position:absolute;left:0;bottom:45px');
		span.innerHTML=text;
	div.appendChild(span);	
	
	var onClickText=handler+"('"+tagName+"','"+tagValue+"','"+itemDescr+"')";
	
	var button=document.createElement('button');
		button.setAttribute('class','w3-border w3-border-red w3-round w3-green');
		button.setAttribute('onclick',onClickText);
		button.innerHTML='&#9986';
	div.appendChild(button);		
	
	return div;
}

function createItemEditButton(tagName,tagValue,itemDescr,headerValue){
	var div=document.createElement('div');
		div.setAttribute('class','w3-tooltip');
		div.setAttribute('style','margin-top: 10px; margin-left: 3px; width: 30px');
	
	var text='&#10162 Редактирование';
	if(itemDescr!=undefined){
		text+=' ';
		text+=itemDescr;
	}
	
	if(headerValue==undefined || headerValue==''){
		headerValue=text;
	}
		
	var span=document.createElement('span');
		span.setAttribute('class','w3-text w3-tag w3-gray w3-round');
		span.setAttribute('style','position:absolute;left:0;bottom:45px');
		span.innerHTML=text;
	div.appendChild(span);	
	
	var onClickText="openItemPopup('"+tagName+"','"+tagValue+"','"+headerValue+"')";
	
	var button=document.createElement('button');
		button.setAttribute('class','w3-border w3-border-red w3-round w3-green');
		button.setAttribute('onclick',onClickText);
		button.innerHTML='&#9997';
	div.appendChild(button);		
	
	return div;
}        

function createInputComponent(id1,id2,fieldIntId,extraClass,fieldVal,sensorName,itemName,noId,editable){
	var compId=getInputCompName(id1,id2,fieldIntId);
	
	var input1=document.createElement("Input");
		
		input1.setAttribute("id",compId+"_"+noId);
		input1.setAttribute("name",compId);
		input1.setAttribute("class","w3-input w3-border "+extraClass);
		input1.setAttribute("style",'width: 90%;');
		input1.setAttribute("value",fieldVal);
		if(sensorName!=undefined){
			input1.setAttribute(SENSOR_NAME_ATTR,sensorName);
		}
		if(editable==undefined || !editable)
			input1.setAttribute("disabled","disabled");
		if(itemName!=undefined)
			input1.setAttribute(ITEM_NAME_ATTR,itemName);	
		
	return input1; 
}

function createHeaderElement(elType,elStyle,elText){
	var itemHeader = document.createElement(elType);
	if(elStyle!=undefined && elStyle!='')
		itemHeader.setAttribute('style',elStyle);
	var textNode = document.createTextNode(elText);
	itemHeader.appendChild(textNode);
	
	return itemHeader;
}

function getInputCompName(sensorName,itemName,suffix){
	return SENSORS_SHORT+sensorName+"_"+itemName+"_"+suffix;
}

/*-------------------form submit-----------*/
function submitCurrentSettingsForm(){
	showStatusMessage('Сохраняю настройки ...','w3-yellow');
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructFormDataAsJson,undefined,processDeviceSettingsSave,getStatusMessageComp());
}
