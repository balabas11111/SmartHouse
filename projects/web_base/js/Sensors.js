
var snrFields=["id","name","kind","type","size","descr","itemCount"];
var itemFields=["id","name","type","size","descr","val","minVal","maxVal","fieldId","queue"];

function openSensorsTab(){
	preventFormSubmit('sensors_form');
	preventFormSubmit('currentSensor_form');
	
	document.title='Настройки датчиков устройства';

	hideComponentsByClassName('settingsTab');
	document.getElementById('sensors').style.display = "block";
	
	setStatusMessageComp(document.getElementById('sensors_msg'));
	showStatusMessage('Загружаю настройки датчиков...','w3-yellow');
	
	var url='/getJson?name=deviceHelper&page=sensors';
	updateComponentsByAjaxCall('GET', url, processSensorsJsonGet,"",undefined, 0,2000);
}
function processSensorsJsonGet(data){
	var container=document.getElementById('sensors_form');
	var countDiv=document.getElementById('sensors_count');
	
	var sensors=data.sensors;
	var count=sensors.length;
	
	countDiv.innerHTML=(count>0)?('Всего датчиков '+count):'Не подключено ни одного датчика';
	container.innerHTML='';
	
	var templateSensorName='sensor-header-template';
	var templateItemName='sensor-item-template';
			
	for(var i in sensors){
		var sensor=sensors[i];
		var suffix='_'+sensor.name;
		var sensorDiv=createHtmlComponent('div','sensor_'+sensor.name,'','','',' ');
		var sensorHeadDiv=createHtmlComponent('div','sensor_header_'+sensor.name,'','','',' ');
		var itemsDiv=createHtmlComponent('div','sensor_items_'+sensor.name,'','','',' ');
		
		putSensorHeaderToTemplate(templateSensorName,sensorHeadDiv,sensor,true,suffix);
		
		for(var j in sensor.items){
			var item=sensors[i].items[j];
			putSensorItemToTemplate(templateItemName,itemsDiv,item,true,suffix+'_'+j);
		}
		
		sensorDiv.append(sensorHeadDiv);
		sensorDiv.append(itemsDiv);
		
		container.append(sensorDiv);
	}
	
	var msg=(customOnGetMessage!=undefined)?customOnGetMessage:('Датчики загружены');
	
	showStatusMessage(msg,'w3-green');
	customOnGetMessage=undefined;
}
function showEditSensorForm(sensorName){
	var sensor=sensorFromHtml('_'+sensorName);
		
	var header='➲ Редактирование : '+sensor.name+'('+sensor.descr+')';
	openSensorPopup(sensor,header);
}
function sensorFromHtml(presuffix){
	var sensor={};
	sensor.items=[];
	
	for(var i=0;i<snrFields.length;i++){
		sensor[snrFields[i]]=getComponentValue(document.getElementById('sensor_'+snrFields[i]+presuffix));
	}
	
	for(var i=0;i<sensor.itemCount;i++){
		var item={};
		for(var j=0;j<itemFields.length;j++){
			var elId='sensor_item_'+itemFields[j]+presuffix+'_'+i;
			item[itemFields[j]]=getComponentValue(document.getElementById(elId));
		}
		sensor.items[i]=item;
	}
	return sensor;
}
function openSensorPopup(sensor,header){
	var suffix='';
	var templateName='sensors_'+sensor.name;
	
	var headComp=getComponentById('currentSensor_header');
	var container=getComponentById('currentSensor_content');
	var msgComp=getComponentById('currentSensor_msg');
	
	headComp.innerHTML=header;
	container.innerHTML='';
	
	msgComp.innerHTML='';
	setStatusMessageComp(msgComp);
	
	var templateSensorName='sensor-header-template';
	var templateItemName='sensor-item-template';
	
	var sensorDiv=createHtmlComponent('div','popup_'+sensor.name,'','','',' ');
	
	putSensorHeaderToTemplate(templateSensorName,sensorDiv,sensor,false,suffix);
	
	for(var j in sensor.items){
		var item=sensor.items[j];
		putSensorItemToTemplate(templateItemName,sensorDiv,item,false,suffix+'_'+j);
	}
	
	container.append(sensorDiv);
	
	showComponent('currentSensor_modal');
}
function putSensorHeaderToTemplate(templateName,container,sensor,disabled,suffix){
	var clazz='sensor';
	var template=document.getElementById(templateName).content.cloneNode(true);
	
	putValuesToForm(template,container,sensor,clazz,snrFields,disabled,suffix,true);
}
function putSensorItemToTemplate(templateName,container,item,disabled,suffix){
	var clazz='sensor_item';
	var template=document.getElementById(templateName).content.cloneNode(true);
	
	putValuesToForm(template,container,item,clazz,itemFields,disabled,suffix,false);
}
function putValuesToForm(template,container,value,clazz,fields,disabled,suffix,btns,prefix){
	template.querySelector('div.'+clazz).id = clazz+suffix;
	
	if(btns){
		if(!disabled){
			var editBtn=template.querySelector('div.editBtn');
			editBtn.parentNode.removeChild(editBtn);
		}else{	
			template.querySelector('span.editBtn').innerText = '➲ Редактирование '+value.name+' ('+value.descr+')';
			
			template.querySelector('button.editBtn').setAttribute('onclick',"showEditSensorForm('"+value.name+"');");
		}
	}
	
	for(var i=0;i<fields.length;i++){
		updateTemplateValue(template,clazz,fields[i],value[fields[i]],suffix,disabled,prefix);
	}

	container.appendChild(template);
}

function submitSensorsFormAsJsonReloadCurrTab(){
	showStatusMessage('Сохраняю датчики ...','w3-yellow');
	
	var form=document.getElementById('currentSensor_form');
	var submitSensorsUrl='/processJson?name=deviceHelper&page=sensors';
	
	postForm(form,submitSensorsUrl,validateCurrentSensorForm,constructSensorsFormDataAsJson,validateStatusMessageDefault,onSensorsSavedHandler,getStatusMessageComp());
}

function onSensorsSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		hideComponent('currentSensor_modal');
		openSensorsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
		/*openCommandRestartPopup();*/
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function constructSensorsFormDataAsJson(form){
	
	var sensor=sensorFromHtml('');
	var target=getComponentValueById('currentSensor_form_target');
	var page=getComponentValueById('currentSensor_form_page');
	
	var json=JSON.stringify(sensor);
	
	return constructFormData_JSONprocessor(target,page,json);
}

/*------------------------validators------------------------------------*/
function validateCurrentSensorForm(){
	var sensorName=getComponentValueById('sensor_name');
	var itemCount=getComponentValueById('sensor_itemCount');
	
	var errorMessage='';
	
	var descr=document.querySelector('#sensor_descr');
	
	errorMessage+=validateFieldUniqueByValue(descr,'.sensor.descr:not(#sensor_descr_'+sensorName+')','Поле Имя не уникальное <br>',false);
	
	for(var i=0;i<itemCount;i++){
		var itemDescr=document.querySelector("#sensor_item_descr_"+i);
		errorMessage+=validateFieldUniqueByValue(itemDescr,".sensor_item.descr:not(#sensor_item_descr_"+i+"):not(#sensor_item_descr_"+sensorName+"_"+i+"):not([value=''])",'Канал ThingSpeak не уникален <br>',true);
		var itemFieldId=document.querySelector('#sensor_item_fieldId_'+i);
		
		if(itemFieldId.value!='0'){
			errorMessage+=validateFieldUniqueByValue(itemFieldId,'.sensor_item.fieldId:not(#sensor_item_fieldId_'+i+'):not(#sensor_item_fieldId_'+sensorName+'_'+i+'):not([value=0])','Канал ThingSpeak не уникален <br>');
		}
		
		var min=document.querySelector('#sensor_item_minVal_'+i);
		var max=document.querySelector('#sensor_item_maxVal_'+i);
		
		markComponentValidity(min,true);
		markComponentValidity(max,true);
		
		if(min.value>=max.value){
			markComponentValidity(min,false);
			markComponentValidity(max,false);
			
			errorMessage+='Мин/макс значения неверны <br>'
		}
	}
	return errorMessage;
}
function validateFieldUniqueByValue(input,otherSelr,errMsg,required){
	var result='';
	
	/*component in edit form*/
	var inputVal=getComponentValue(input);
	
	markComponentValidity(input,true);
	
	if(required){
		if(inputVal==undefined || inputVal==''){
			markComponentValidity(input,false);
			return 'Пустое значение не допустимо <br>';
		}
	}
	
	/*component in list*/
	var inputs=document.querySelectorAll(otherSelr);
	
	for(var i=0;i<inputs.length;i++){
		if(inputs[i].id!=input.id && inputVal==getComponentValue(inputs[i])){
			result+=errMsg;
			markComponentValidity(input,false);
		}
	}
	return result;
}
