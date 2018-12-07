//------------------------------Base logic----------------------------
function onLoadPageComplete(){
	document.title='Настройки устройства';
	w3_close();
}
//----------------------------------------------------------------------
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

var getValuesHandler=undefined;
var validateValuesHandler=undefined;

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
	currentForm=getComponentById(getComponentIdWithSuffix(tabName,FORM_SUFFIX));
	
	//submitValuesUrl='/submitForm_'+tabName;
	//getValuesUrl='/getJson_'+tabName;
	
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
			validateValuesHandler=undefined;
			submitValuesUrl='/submitForm_'+'settings';
			getValuesUrl='/getJson_'+'settings?page='+tabName;
		}
		
		if(tabName=='sensors'){
			currentForm.innerHTML="";
			getValuesHandler=processSensorsJsonGet;
			validateValuesHandler=validateSensorsPage;
			submitValuesUrl='/submitForm_'+'sensors';
			getValuesUrl='/getJson_'+'sensors';
		}
		
		if(tabName=='publish'){
			getValuesHandler=processDeviceSettingsGet;
			validateValuesHandler=undefined;
			submitValuesUrl='/submitForm_'+'settings';
			getValuesUrl='/getJson_'+'settings?page='+tabName;
		}

		if(getValuesHandler!=undefined){
			currentForm.addEventListener('submit', 
								function(evt){
									evt.preventDefault();
								},false);
			
			updateComponentsByAjaxCall('GET', getValuesUrl, getValuesHandler,"",undefined, 0);
		}else{
			alert('Error on page!');
		}
	}
}

//---------------------------------device settings tab----------------------
function processDeviceSettingsGet(data){
	processSimpleJsonResponse(data,'');
	showMessage(currentMessageComp,'Загружено '+currentHeaderName,'w3-green');
}

function validateDeviceSettingsForm(){
	var errorMessage='';
	
	var pass=document.getElementById('espSettingsBox_accessPass');
	var conf=document.getElementById('espSettingsBox_accessPassConfirm');
	
	errorMessage=processPassConfirmPath(pass,conf);
	
	pass=document.getElementById('espSettingsBox_settingsPass');
	conf=document.getElementById('espSettingsBox_settingsPass');
	
	errorMessage=errorMessage+processPassConfirmPath(pass,conf);
	
	if(!pass || pass.value==undefined || 0===pass.length){
		errorMessage='Доступ на страницу пароля возможен только по паролю!';
	}
			
	return errorMessage;
}

function processPassConfirmPath(pass,conf){
	var message="";
	
	markComponentAs_Valid(pass);
	markComponentAs_Valid(conf);
	
	if(pass.value!=conf.value){
		markComponentAs_InValid(pass);
		markComponentAs_InValid(conf);
		
		message="Пароль и подьверждение пароля не равны";
	}
	
	return message;
}

function ValidateIPaddress(ipaddress) {  
	  if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(ipaddress)) {  
	    return (true)  
	  }  
	  alert("You have entered an invalid IP address!")  
	  return (false)  
	}
//-------------------------------------------------------
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
	
	postForm(form,'/submitForm_commands',undefined,getCurrentCommandHandler);
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

//-------------------------------------------------------
function processSensorsJsonGet(data){
	var container=currentForm;

	container.innerHTML='';
	var sensors=data.sensors;
	
	currentJson=sensors;
			
	for(var s in sensors){
		var sensor=sensors[s];
		
		putSensorContentToContainer(container,sensor,false,false);
	}
	
	showMessage(currentMessageComp,'Загружено '+currentHeaderName,'w3-green');
}

function putSensorContentToContainer(container,sensor,noId,editable){
	var sensorId=sensor.id;
	var sensorName=sensor.name;
	var sensorType=sensor.type;
	var sensorSize=sensor.size;
	var sensorDescr=sensor.descr;
		
	var hr = document.createElement('hr');
	container.appendChild(hr);
	container.appendChild(hr);
	//sensor header 
	var divh=document.createElement('div');
	divh.setAttribute('class','w3-row w3-teal');
	divh.setAttribute('style','margin-top: 15px; height: 50px;');
	
	var div1h=createDivComponent('w3-quarter','Название устройства');
	var div2h=createDivComponent('w3-quarter','Тип устр.');
	var div3h=createDivComponent('w3-quarter','Измеряемые величины');
	var div4h=createDivComponent('w3-quarter','Описание устройства');
	
	divh.appendChild(div1h);
	divh.appendChild(div2h);
	divh.appendChild(div3h);
	divh.appendChild(div4h);
	
	container.appendChild(divh);
	//sensor own values
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 44px;');
	
	var col1h=createDivComponent('w3-quarter','');
	var col2h=createDivComponent('w3-quarter','');
	var col3h=createDivComponent('w3-quarter','');
	var col4h=createDivComponent('w3-quarter','');
	
	var editButton=createSensorEditButton(sensorName,sensorDescr);
	var sName=createHeaderElement('H4','margin-left: 20px;',sensorName)
	var sType=createHeaderElement('H4','margin-left: 20px;',sensorType)
	var sSize=createHeaderElement('H4','margin-left: 20px;',sensorSize)
	var sDescr=createInputComponent(sensorId,255,FIELD_DESCR_ID,DESCR_SUFFIX,sensorDescr,sensorName,itemName,noId,editable);
	
	if(editable==undefined || !editable){
		col1h.appendChild(editButton);
	}
	col1h.appendChild(sName);
	col2h.appendChild(sType);
	col3h.appendChild(sSize);
	col4h.appendChild(sDescr);
					
	divhv.appendChild(col1h);
	divhv.appendChild(col2h);
	divhv.appendChild(col3h);
	divhv.appendChild(col4h);
	
	container.appendChild(divhv);
	
	//sensor children header
	container.appendChild(hr);
	
	var div=document.createElement('div');
	div.setAttribute('id',sensorName+'_div');
	div.setAttribute('class','w3-row w3-teal');
	div.setAttribute('style','margin-top: 2px;');
	
	var div1=createDivComponent('w3-threequarter');
	var div2=createDivComponent('w3-quarter');
	
	var col0=createDivComponent('w3-third',' Датчики устройства');
	var col1=createDivComponent('w3-third','Описание');
	var col2=createDivComponent('w3-third','Очередь');
	
	var col3=createDivComponent('w3-third','Миним. значение');
	var col4=createDivComponent('w3-third','Макс значение');
	var col5=createDivComponent('w3-third','Поле ThingSpeak');
	
	col0.setAttribute('style','height: 50px;');
	col1.setAttribute('style','height: 50px;');
	col2.setAttribute('style','height: 50px;');
	col3.setAttribute('style','height: 50px;');
	col4.setAttribute('style','height: 50px;');
	col5.setAttribute('style','height: 50px;');
	
	div1.appendChild(col0);
	div1.appendChild(col1);
	div1.appendChild(col2);
	div2.appendChild(col3);
	div2.appendChild(col4);
	div2.appendChild(col5);
	
	div.appendChild(div1);
	div.appendChild(div2);
	container.appendChild(div);
	//sensor children values
	var items=sensor.items;
	
	for(var i in items){
		var itemId=items[i].id;
		var itemName=items[i].name;
		var descr=items[i].descr;
		var minVal=items[i].minVal;
		var maxVal=items[i].maxVal;
		var fieldId=items[i].fieldId;
		var queue=items[i].queue;
		
			var div=document.createElement('div');
			div.setAttribute('class','w3-row');
			div.setAttribute('style','margin-top: 5px;');
			
			var div1=createDivComponent('w3-threequarter','');
			var div2=createDivComponent('w3-quarter','');
			
			var col0=createDivComponent('w3-third','');
			var col1=createDivComponent('w3-third','');
			var col2=createDivComponent('w3-third','');
								
			var col3=createDivComponent('w3-third','');
			var col4=createDivComponent('w3-third','');
			var col5=createDivComponent('w3-third','');
			
			var text0=document.createElement('div');
			text0.setAttribute('class','w3-half');
			text0.setAttribute('style','margin-left: 20px; width: 90%;');
			text0.innerHTML="<b>"+itemName+"</b>";

			var input1=createInputComponent(sensorId,itemId,FIELD_DESCR_ID,DESCR_SUFFIX,descr,sensorName,itemName,noId,editable);
			var input2=createInputComponent(sensorId,itemId,FIELD_QUEUE_ID,QUEUE_SUFFIX,queue,sensorName,itemName,noId,editable);
			var input3=createInputComponent(sensorId,itemId,FIELD_MIN_ID,MIN_VAL_SUFFIX,minVal,sensorName,itemName,noId,editable);
			var input4=createInputComponent(sensorId,itemId,FIELD_MAX_ID,MAX_VAL_SUFFIX,maxVal,sensorName,itemName,noId,editable);
			var input5=createInputComponent(sensorId,itemId,FIELD_FIELDID_ID,FIELD_ID_SUFFIX,fieldId,sensorName,itemName,noId,editable);
			
			input5.setAttribute('type','number');
			input5.setAttribute('min','0');
			input5.setAttribute('max','');
			input5.setAttribute('step','1');
			
			col0.appendChild(text0);
			col1.appendChild(input1);
			col2.appendChild(input2);
			col3.appendChild(input3);
			col4.appendChild(input4);
			col5.appendChild(input5);
								
			div1.appendChild(col0);
			div1.appendChild(col1);
			div1.appendChild(col2);
			
			div2.appendChild(col3);
			div2.appendChild(col4);
			div2.appendChild(col5);
			
			div.appendChild(div1);
			div.appendChild(div2);
			
			container.appendChild(div);
		
		//end of items loop
	}
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function validateCurrentSensorForm(){
	var errorMessage='';
	var validateForm=getComponentById('currentSensor_form');
		
	errorMessage=errorMessage+validateFieldValuesUnique(validateForm,QUEUE_SUFFIX,"Неверное значение 'Очередь'; <br>","Поле 'Очередь' не уникальное; <br>",false);
	errorMessage=errorMessage+validateFieldValuesUnique(validateForm,FIELD_ID_SUFFIX,"Неверное значение 'Поле ThingSpeak'; <br>","'Поле ThingSpeak' не уникальное; <br>",true);
	errorMessage=errorMessage+validateMinMaxValues();
	errorMessage=errorMessage+validateDescrValues();
	
	return errorMessage;
}

function getCurrentSensorFromCurrentJson(sensorName){
	for(var i in currentJson){
		if(currentJson[i].name==sensorName){
			return currentJson[i];
		}
	}
	
	return undefined;
}

function getCurrentItemFromSensor(sensor,itemName){
	for(var i in sensor.items){
		if(sensor.items[i].name==itemName){
			return sensor.items[i];
		}
	}
	
	return undefined;
}

function openSensorsPopup(sensorName){
	var sensor=getCurrentSensorFromCurrentJson(sensorName);
	currentMessageComp=getComponentById('currentSensor_msg');
	
	if(sensor!=undefined){
		var sensorNameContainer=getComponentById('currentSensor_name');
		var sensorNameHeader=getComponentById('currentSensor_header');
		var container=getComponentById('currentSensor_content');
		
		sensorNameContainer.value=sensorName;
		sensorNameHeader.innerHTML=sensorName;
		container.innerHTML='';
		
		putSensorContentToContainer(container,sensor,true,true);
		hideComponent('currentSensor_msg');
		showComponent('currentSensor_modal');
	}
}

function closeSensorsPopup(){
	hideComponent('currentSensor_modal');
	currentMessageComp=getComponentById('sensors_msg');
}

function submitCurrentSensorForm(){
	var form=getComponentById('currentSensor_form');
	
	showMessage(currentMessageComp,'Сохраняю настройки датчика...','w3-yellow');
	
	postForm(form,submitValuesUrl,validateCurrentSensorForm,getCurrentSensorHandler);
}

function getCurrentSensorHandler(data){
	if(data.status=='Ok'){
		hideComponent('currentSensor_modal');
		openTab(currentTab,currentHeaderName);
	}else{
		showMessage(currentMessageComp,'Ошибка '+data.message,'w3-red');
	}
}

function createSensorEditButton(sensorName,sensorDescr){
	
	/*var html=`<div class="w3-tooltip">
					<span style="position:absolute;left:0;bottom:40px" class="w3-text w3-tag w3-gray w3-round">
						<i class="material-icons">help_outline</i>
	  						Редактировать <sensorDescr>
						</span>
					<button class="w3-button w3-white w3-border w3-border-red w3-round-large" onclick="openSensorsPopup('<sensorName>')">
	  	 				<i class="fa fa-edit"></i>
					</button>
			</div>`;
	  
	html=html.replace('<sensorName>',sensorName);
	html=html.replace('<sensorDescr>',sensorDescr);
	
	var div=createDivComponent('w3-third',html);
	  
	return div;
	*/
	var div=document.createElement('div');
	div.setAttribute('class','w3-third');
	
	var div2=document.createElement('div');
		div2.setAttribute('class','w3-tooltip');
		
	var span=document.createElement('span');
		span.setAttribute('class','w3-text w3-tag w3-gray w3-round');
		span.setAttribute('style','position:absolute;left:0;bottom:40px');
	div2.appendChild(span);	
	
	var iel=document.createElement('i');
		iel.setAttribute('class','material-icons');
		iel.innerHTML='help_outline';
	
	var text='Редактировать '+sensorDescr;
	var content = document.createTextNode(text);
	div2.appendChild(content);
	
	var onClickText="openSensorsPopup('"+sensorName+"')";
	
	var button=document.createElement('button');
		span.setAttribute('class','w3-button w3-white w3-border w3-border-red w3-round-large');
		span.setAttribute('onclick',onClickText);
	div2.appendChild(button);		
	
	div.appendChild(div2);
	  
	return div;
}

function validateSensorsPage(){
	var errorMessage='';
	
	
	return errorMessage;
}

//------------------------validators------------------------------------
function validateMinMaxValues(){
	var minValComponents=document.getElementsByClassName(MIN_VAL_SUFFIX);
	var maxValComponents=document.getElementsByClassName(MAX_VAL_SUFFIX);
	var result='';
	var error=false;
	
	for (var i in minValComponents) {
		var comp=minValComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			markComponentAs_Valid(comp);
		}
	}
	
	for (var i in maxValComponents) {
		var comp=maxValComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			markComponentAs_Valid(comp);
		}
	}
		
	for (var i in minValComponents) {
		var comp=minValComponents[i];
		var compId=comp.id;
		if(compId!=undefined){
			var compVal=comp.value;
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
			
			var maxComp=getMaxComponent(sensorName,itemName);
			var maxCompVal=maxComp.value;
				
			if(compVal>=maxCompVal){
				markComponentAs_InValid(comp);
				markComponentAs_InValid(maxComp);
				
				error=true;
				
			}
		}
	}
	
	if(error){
		result="Неверное значение 'Мин/Макс' мин должно быть меньше макс; <br>";
	}

	return result;
}

function getMaxComponent(sensorName,itemName){
	var components=document.getElementsByClassName(MAX_VAL_SUFFIX);
	
	for (var i in components) {
		var comp=components[i];
		var compId=comp.id;
		var compVal=comp.value;
		
		var sensorName2=comp.getAttribute(SENSOR_NAME_ATTR);
		var itemName2=comp.getAttribute(ITEM_NAME_ATTR);
				
		if(sensorName2==sensorName2 && itemName==itemName2){
			
			return comp;
		}
	}
}

function validateDescrValues(){
	var descrComponents=document.getElementsByClassName(DESCR_SUFFIX);
	var result='';
	var error=false;
	
	for (var i in descrComponents) {
		var comp=descrComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			markComponentAs_Valid(comp);
		}
	}
	
	for (var i in descrComponents) {
		var comp=descrComponents[i];
		var compId=comp.id;
		if(compId!=undefined){
			var compVal=comp.value;
			
			if(compVal==''){
				markComponentAs_InValid(comp);
				var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
				var itemName=comp.getAttribute(ITEM_NAME_ATTR);
				error=true;
			}
		}
	}
	
	if(error){
		result="Пустое значение поля'Описание'; <br>";
	}

	return result;
}

function validateFieldValuesUnique(validateComnponent,validClassName,errorMessage1,errorMessage2, notAllowEmpty){
	var result='';
	var error=false;
	var fieldIdComponents=validateComnponent.getElementsByClassName(validClassName);
	var fieldIdComponents2=document.getElementsByClassName(validClassName);
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			markComponentAs_Valid(comp);
		}
	}
	
	for (var i in fieldIdComponents2) {
		var comp=fieldIdComponents2[i];
		if(comp!=undefined && comp.classList!=undefined){
			markComponentAs_Valid(comp);
		}
	}
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		var compId=comp.id;
		
		if(compId!=undefined){
			var compVal=comp.value;
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
					
			if((!compVal || 0 === compVal.length) || compVal<0){
				
				if(notAllowEmpty){
									
					markComponentAs_InValid(comp);
					error=true;
					result=errorMessage1;
				}
			}else{
				if(compVal!='0'){	
					for (var j in fieldIdComponents2) {
						var comp2=fieldIdComponents2[j];
						var compId2=comp2.id;
						
						if(compId!=undefined && compId2!=undefined){
							var compVal2=comp2.value;
							
							var sensorName2=comp2.getAttribute(SENSOR_NAME_ATTR);
							var itemName2=comp2.getAttribute(ITEM_NAME_ATTR);
							
							if(compId!=compId2 && sensorName!=sensorName2 && itemName!=itemName2 && compVal==compVal2){
								
								var sensor=getCurrentSensorFromCurrentJson(sensorName2);
								var item=getCurrentItemFromSensor(sensor,itemName2);
								
								if(sensor!=undefined && item!=undefined){
									result+=' Значение ="'+compVal2+'" уже используется: '+' Устройство "'+sensor.descr+'"'+'Датчик "'+item.descr+'";';
								}
																
								markComponentAs_InValid(comp);
								markComponentAs_InValid(comp2);
								
								error=true;
								
							}
						
						}
						
					}
				}
			}
		}
	}
	
	if(error){
		result=errorMessage2+result;
	}
	
	return result;
}

//--------------------------GUI creation part-----------------------------
function createDivComponent(className,innerHtml){
	var div=document.createElement('div');
		div.setAttribute('class',className);
		if(innerHtml!=undefined && innerHtml!=''){
			div.innerHTML=innerHtml;
		}
		
	return div;
}

function createInputComponent(id1,id2,fieldIntId,suffixName,fieldVal,sensorName,itemName,noId,editable){
	var compId=getInputCompName(id1,id2,fieldIntId);
	
	var input1=document.createElement("Input");
		
		input1.setAttribute("id",compId+"_"+noId);
		input1.setAttribute("name",compId);
		input1.setAttribute("class","w3-input w3-border "+suffixName);
		input1.setAttribute("style",'width: 90%;');
		input1.setAttribute("value",fieldVal);
		input1.setAttribute(SENSOR_NAME_ATTR,sensorName);
		if(editable==undefined || !editable)
			input1.setAttribute("disabled","disabled");
		if(itemName!=undefined)
			input1.setAttribute(ITEM_NAME_ATTR,itemName);	
		
	return input1; 
}

function createHeaderElement(elType,elStyle,elText){
	var sensorHeader = document.createElement(elType);
	if(elStyle!=undefined && elStyle!='')
		sensorHeader.setAttribute('style',elStyle);
	var textNode = document.createTextNode(elText);
	sensorHeader.appendChild(textNode);
	
	return sensorHeader;
}

function getInputCompName(sensorName,itemName,suffix){
	return SENSORS_SHORT+sensorName+"_"+itemName+"_"+suffix;
}

//--------------------form submit
function submitCurrentForm(){
	postForm(currentForm,submitValuesUrl,validateValuesHandler,getValuesHandler);
}
