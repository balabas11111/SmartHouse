/*----------------------------------------------------------------------*/
const FIELD_DESCR_ID   = 1;
const FIELD_MIN_ID     = 2;
const FIELD_MAX_ID     = 3;
const FIELD_FIELDID_ID = 4;
const FIELD_QUEUE_ID   = 5;

const SENSOR_NAME_ATTR='sensorname';
const ITEM_NAME_ATTR='itemname';

const SENSORS="sensors_";
const SENSORS_SHORT="s_";

var currentTab='';
var currentHeaderName='';

var currentMessageComp=undefined;
var currentForm=undefined;
var currentFormList=undefined;

var getValuesHandler=undefined;
var validateValuesHandler=undefined;

var putItemsHeaderToContainer=undefined;
var putItemsToContainerHandler=undefined;
var currentItemPreffix=undefined;
var itemsTagName=undefined;

var getValuesUrl='';
var submitValuesUrl='';

var currentJson='';


function openTab(tabName,headerName) {
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
	
	currentMessageComp=getComponentById(getComponentIdWithSuffix(tabName,MSG_SUFFIX));
	
	var currentFormId=getComponentIdWithSuffix(tabName,FORM_SUFFIX);
	
	document.title='Настройки устройства - '+currentHeaderName;
	
	if(containerComponent.classList.contains('reloadableSettingsContainer')){
				
		
		showMessage(currentMessageComp,loadMessagePreffix+' информацию о датчиках...','w3-yellow');
		
		var handler=undefined;
		
		if(tabName=='device'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateDeviceSettingsForm;
			submitValuesUrl='/submitForm_'+'settings';
			getValuesUrl='/getJson_'+'settings?page='+tabName;
		}
		
		if(tabName=='net'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/submitForm_'+'settings';
			getValuesUrl='/getJson_'+'settings?page='+tabName;
		}
		
		if(tabName=='sensors'){
			getValuesHandler=processItemsJsonGet;
			putItemsHeaderToContainer=undefined;
			putItemsToContainerHandler=putSensorContentToContainer;
			currentItemPreffix='currentSensor';
			itemsTagName='sensors';
			validateValuesHandler=validateCurrentSensorForm;
			submitValuesUrl='/submitForm_'+'sensors';
			getValuesUrl='/getJson_'+'sensors';
			currentFormId=getComponentIdWithSuffix(currentItemPreffix,FORM_SUFFIX);
		}
		
		if(tabName=='intervals'){
			getValuesHandler=processIntervalsJsonGet;
			putItemsHeaderToContainer=putIntervalHeaderToContainer;
			putItemsToContainerHandler=putIntervalContentToContainer;
			currentItemPreffix='currentInterval';
			itemsTagName='intervals';
			validateValuesHandler=validateCurrentIntervalForm;
			submitValuesUrl='/processJson?name=TimeIntervalService';
			getValuesUrl='/getJson?name=TimeIntervalService';
			currentFormId=getComponentIdWithSuffix(currentItemPreffix,FORM_SUFFIX);
		}
		
		if(tabName=='publish'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/submitForm_'+'settings';
			getValuesUrl='/getJson_'+'settings?page='+tabName;
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

/*---------------------------------device settings tab----------------------*/
function processDeviceSettingsGet(data){
	processSimpleJsonResponse(data,'');
	showMessage(currentMessageComp,'Загружено '+currentHeaderName,'w3-green');
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

function openConfirmPopup(message,action){
	
	var actionCommand="submitCommand('"+action+"')";
	showComponent('confirm_headerCloseBtn');
	showComponent('confirm_okBtn');
	showComponent('confirm_cancelBtn');
	hideComponent('confirm_closeBtn');
	
	getComponentById('confirm_header').innerHTML='Подтвердите действие';
	
	getComponentById('confirm_content').innerHTML=message;
	getComponentById('confirm_okBtn').setAttribute('onclick',actionCommand);
	showComponent('confirm_modal');
}

function submitCommand(command){
	getComponentById('confirm_command').value=command;
	currentMessageComp=undefined;
	
	var form=getComponentById('confirm_form');
	var msg=getComponentById('confirm_msg');
	
	showMessage(msg,'Отправляю команду '+command+' на устройство...','w3-yellow');
	
	postForm(form,'/submitForm_commands',undefined,constructFormDataDefault,getCurrentCommandHandler,msg);
}

function getCurrentCommandHandler(data){
	var status=data.status;
	var message=data.message;
	var messageColorClass='w3-yellow';
	
	
	if(status=='Ok'){
		messageColorClass='w3-green';
	}
	if(status=='Error' || status=='NotFound'){
		messageColorClass='w3-red';
	}
	
	var msg=getComponentById('confirm_msg');
	showMessage(msg,' Сообщение '+message,messageColorClass);
	
	var command=getComponentById('confirm_command').value;
	
	if(command=='restart'){
		getComponentById('confirm_header').innerHTML='Перезагрузка устройства';
		
		hideComponent('confirm_headerCloseBtn');
		hideComponent('confirm_okBtn');
		hideComponent('confirm_cancelBtn');
		hideComponent('confirm_closeBtn');
		
		setTimeout(function(){tickRestart();}, 1000);
	}
	if(command=='recreateThingSpeak'){
		var headMess=getComponentById('confirm_header').innerHTML='Сообщение системы';
		
		var result='Каналы и очереди ThingSpeak были пересозданы.<br> Перезапустите устройство для применения новых настроек.<br> Все предыдущие данные каналов находятся в старых каналах и могут быть просмотрены<br>'+
					'Создано : '+message;
		
		getComponentById('confirm_closeBtn').setAttribute('onclick',"displayRestartDialog();");
		getComponentById('confirm_closeBtn').innerHTML="Закрыть и перезапустить";
		
		hideComponent('confirm_headerCloseBtn');
		hideComponent('confirm_okBtn');
		hideComponent('confirm_cancelBtn');
		showComponent('confirm_closeBtn');
						
		hideComponent('confirm_msg');
		getComponentById('confirm_content').innerHTML=result;
	}
	
	if(command=='deleteSettings'){
		var headMess=getComponentById('confirm_header').innerHTML='Сообщение системы';
		
		var result='Все настройки системы были удалены<br> Перезапустите устройство для применения настроек по умолчанию. <br>'+message+'<br>';
		
		getComponentById('confirm_closeBtn').setAttribute('onclick',"displayRestartDialog();");
		getComponentById('confirm_closeBtn').innerHTML="Закрыть и перезапустить";
		
		hideComponent('confirm_headerCloseBtn');
		hideComponent('confirm_okBtn');
		hideComponent('confirm_cancelBtn');
		showComponent('confirm_closeBtn');
		
		hideComponent('confirm_msg');
		getComponentById('confirm_content').innerHTML=result;
	}
}

function displayRestartDialog(){
	openConfirmPopup('Подтвердите рестарт устройства','restart');
}

var restartTimer=30;

function tickRestart(){
	restartTimer--;
	
	if(restartTimer<=0){
		location.reload();
	}
		
	var result='Устройство перезапустится через '+restartTimer+' сек';
	
	hideComponent('confirm_msg');
	getComponentById('confirm_content').innerHTML=result;
	
	setTimeout(function(){tickRestart();}, 1000);
}

function closePopup(modalName,messageComponentName){
	hideComponent(modalName);
	currentMessageComp=getComponentById(messageComponentName);
}

function processItemsJsonGet(data){
	var container=currentFormList;

	container.innerHTML='';
	var items=data[itemsTagName];
	
	currentJson=items;
	
	if(putItemsHeaderToContainer!=undefined){
		putItemsHeaderToContainer(container,items);
	}
			
	for(var s in items){
		var item=items[s];
		
		putItemsToContainerHandler(container,item,false,false);
	}
	
	showMessage(currentMessageComp,'Загружено '+currentHeaderName,'w3-green');
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

function openItemPopup(tagName,sensorName,headerValue){
	var sensor=getCurrentItemFromCurrentJson(tagName,sensorName);
	currentMessageComp=getComponentById(currentItemPreffix+'_msg');
	
	if(sensor!=undefined){
		var sensorNameContainer=getComponentById(currentItemPreffix+'_name');
		var sensorNameHeader=getComponentById(currentItemPreffix+'_header');
		var container=getComponentById(currentItemPreffix+'_content');
		
		sensorNameContainer.value=sensorName;
		sensorNameHeader.innerHTML=headerValue;
		container.innerHTML='';
		
		putItemsToContainerHandler(container,sensor,true,true);
		hideComponent(currentItemPreffix+'_msg');
		showComponent(currentItemPreffix+'_modal');
	}
}

function getCurrentItemHandler(data){
	if(data.status=='Ok'){
		hideComponent(currentItemPreffix+'_modal');
		openTab(currentTab,currentHeaderName);
	}else{
		showMessage(currentMessageComp,'Ошибка '+data.message,'w3-red');
	}
}
/*--------------------------GUI creation part-----------------------------*/
function createItemDeleteButton(tagName,tagValue,itemDescr){
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
	
	var onClickText="openItemDeletePopup('"+tagName+"','"+tagValue+"')";
	
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


function createDivComponent(className,innerHtml){
	var div=document.createElement('div');
	
	if(className!=undefined && className!=''){	div.setAttribute('class',className); }
	if(innerHtml!=undefined && innerHtml!=''){	div.innerHTML=innerHtml; }
		
	return div;
}

function createLabelSimple(id,forVal,clazz,style,value){
	var lbl=document.createElement("label");
	
	lbl.setAttribute("id",id);
	lbl.setAttribute("for",forVal);
	if(clazz!=undefined){ lbl.setAttribute("class",clazz);}
	if(style!=undefined){ lbl.setAttribute("style",style);}
	lbl.innerHTML=value;
	
	return lbl; 
}

function createInputSimple(id,name,clazz,style,value,editable){
	var input1=document.createElement("Input");
	
	input1.setAttribute("id",id);
	input1.setAttribute("name",name);
	if(clazz==undefined){ clazz="w3-input w3-border";}
	input1.setAttribute("class",clazz);
	if(style!=undefined){ input1.setAttribute("style",style);}
	input1.setAttribute("value",value);
	
	if(editable==undefined || !editable)
		input1.setAttribute("disabled","disabled");
	
return input1; 
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
function submitCurrentForm(){
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructFormDataDefault,getValuesHandler,currentMessageComp);
}

function submitCurrentFormReloadCurrTab(){
		showMessage(currentMessageComp,'Сохраняю настройки ...','w3-yellow');
		postForm(currentForm,submitValuesUrl,validateValuesHandler,constructFormDataDefault,getCurrentItemHandler,currentMessageComp);
}

function submitCurrentFormAsJsonReloadCurrTab(){
	showMessage(currentMessageComp,'Сохраняю настройки ...','w3-yellow');
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructFormDataAsJson,getCurrentItemHandler,currentMessageComp);
}


