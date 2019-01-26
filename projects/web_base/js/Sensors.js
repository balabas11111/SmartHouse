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
		var sensorDiv=createHtmlComponent('div','sensors_'+sensor.name,'','','',' ');
		
		putSensorHeaderToTemplate(templateSensorName,sensorDiv,sensor,true,suffix);
		
		for(var j in sensor.items){
			var item=sensors[i].items[j];
			putSensorItemToTemplate(templateItemName,sensorDiv,item,true,suffix+'_'+j);
		}
		
		container.append(sensorDiv);
	}
	
	var msg=(customOnGetMessage!=undefined)?customOnGetMessage:('Датчики загружены');
	
	showStatusMessage(msg,'w3-green');
	customOnGetMessage=undefined;
}
function showEditSensorForm(sensorName){

	var fields=["id","name","kind","type","size","descr","itemCount"];
	var itemFields=["id","name","type","size","descr","val","minVal","maxVal","fieldId","queue"];
	
	var sensor={};
	sensor.items={};
	
	for(var i=0;i<fields.length;i++){
		sensor[fields[i]]=getComponentValue(document.getElementById('sensor_'+fields[i]+'_'+sensorName));
	}
	
	for(var i=0;i<sensor.itemCount;i++){
		var item={};
		for(var j=0;i<itemFields.length;i++){
			var elId='sensor_item'+itemFields[j]+'_'+sensorName+'_'+i;
			item[itemFields[j]]=getComponentValue(document.getElementById(elId));
		}
		sensor.items[i]=item;
	}
		
	var header='➲ Редактирование : '+sensor.name+'('+sensor.descr+')';
	openSensorPopup(sensor,header);
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
	
	putSensorHeaderToTemplate(templateSensorName,container,sensor,false,suffix);
	
	for(var j in sensor.items){
		var item=sensors[i].items[j];
		putSensorItemToTemplate(templateItemName,container,item,false,suffix+'_'+items.name);
	}
	
	showComponent('currentSensor_modal');
}
function putSensorHeaderToTemplate(templateName,container,sensor,disabled,suffix){
	var clazz='sensor';
	var template=document.getElementById(templateName).content.cloneNode(true);
	var fields=["id","name","kind","type","size","descr","itemCount"];
	
	putValuesToForm(template,container,sensor,clazz,fields,disabled,suffix,true);
}
function putSensorItemToTemplate(templateName,container,item,disabled,suffix){
	var clazz='sensor_item';
	var template=document.getElementById(templateName).content.cloneNode(true);
	var itemFields=["id","name","type","size","descr","val","minVal","maxVal","fieldId","queue"];
	
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
	
	postForm(currentForm,submitSensorsUrl,validateCurrentSensorForm,constructSensorsFormDataAsJson,validateStatusMessageDefault,onSensorsSavedHandler,getStatusMessageComp());
}

function onSensorsSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		reloadCurrentSettingsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
		/*openCommandRestartPopup();*/
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function constructSensorsFormDataAsJson(form){
	
	var formInputs = getComponentChildrenByClass(form,'intervals');
	
	var sensorId=getComponentValueById('currentSensor_id');
	var sensorName=getComponentValueById('currentSensor_name');
	var itemCount=getComponentValueById('currentSensor_items');
	
	var target=getComponentValueById('currentSensor_form_target');
	var page=getComponentValueById('currentSensor_form_page');
	
	var sensDescrId=getInputCompName(sensorId,255,FIELD_DESCR_ID,DESCR_SUFFIX)+'_true';
	var sensorDescr=getComponentValueById(sensDescrId);
	
	var json='{"id": "'+sensorId+'","name": "'+sensorName+'","descr": "'+sensorDescr+'","itemCount": "'+itemCount+'","items":[';
	
	for(var i=0;i<itemCount;i++){
		var nameId=getInputCompName(sensorId,i,FIELD_NAME_ID)+'_true';
		var descrId=getInputCompName(sensorId,i,FIELD_DESCR_ID)+'_true';
		var queueId=getInputCompName(sensorId,i,FIELD_QUEUE_ID)+'_true';
		var minValId=getInputCompName(sensorId,i,FIELD_MIN_ID)+'_true';
		var maxValId=getInputCompName(sensorId,i,FIELD_MAX_ID)+'_true';
		var fieldIdId=getInputCompName(sensorId,i,FIELD_FIELDID_ID)+'_true';
		
		var name=getComponentValueById(descrId);
		var descr=getComponentValueById(descrId);
		var queue=getComponentValueById(queueId);
		var minVal=getComponentValueById(minValId);
		var maxVal=getComponentValueById(maxValId);
		var fieldId=getComponentValueById(fieldIdId);
		
		json+='{"id": "'+i+'","name": "'+name+'","descr": "'+descr+'","queue": "'+queue+'","minVal": "'+minVal+'","maxVal": "'+maxVal+'","fieldId": "'+fieldId+'"},';
	}
	
	if(json.substring(json.length - 1)==','){
		json=json.substring(0,json.length-1);
	}
	
	json+=']}';
	
	return constructFormData_JSONprocessor(target,page,json);
}

/*------------------------validators------------------------------------*/
function validateCurrentSensorForm(){
	var errorMessage='';
		
	errorMessage=errorMessage+validateFieldValuesUnique(currentForm,QUEUE_SUFFIX,"Неверное значение 'Очередь'; <br>","Поле 'Очередь' не уникальное; <br>",false);
	errorMessage=errorMessage+validateFieldValuesUnique(currentForm,FIELD_ID_SUFFIX,"Неверное значение 'Поле ThingSpeak'; <br>","'Поле ThingSpeak' не уникальное; <br>",true);
	errorMessage=errorMessage+validateMinMaxValues();
	errorMessage=errorMessage+validateDescrValues();
	
	return errorMessage;
}

function validateMinMaxValues(){
	var minValComponents=document.getElementsByClassName(MIN_VAL_SUFFIX);
	var maxValComponents=document.getElementsByClassName(MAX_VAL_SUFFIX);
	var result='';
	var error=false;
	var comp=undefined;
	
	markComponentsArrayValidity(minValComponents,true);
	markComponentsArrayValidity(maxValComponents,true);
		
	for (var i in minValComponents) {
		comp=minValComponents[i];
		var compId=comp.id;
		if(compId!=undefined){
			var compVal=comp.value;
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
			
			var maxComp=getMaxComponent(sensorName,itemName);
			var maxCompVal=maxComp.value;
				
			if(compVal>=maxCompVal){
				markComponentValidity(comp,false);
				markComponentValidity(maxComp,false);
				
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
	var comp=undefined;
	
	markComponentsArrayValidity(descrComponents,true);
	
	for (var i in descrComponents) {
		comp=descrComponents[i];
		var compId=comp.id;
		if(compId!=undefined){
			var compVal=comp.value;
			
			if(compVal==''){
				markComponentValidity(comp,false);
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
	
	markComponentsArrayValidity(fieldIdComponents,true);
	markComponentsArrayValidity(fieldIdComponents2,true);
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		var compId=comp.id;
		
		if(compId!=undefined){
			var compVal=comp.value;
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
					
			if((!compVal || 0 === compVal.length) || compVal<0){
				
				if(notAllowEmpty){
									
					markComponentValidity(comp,false);
					error=true;
					result=errorMessage1;
				}
			}else{
				if(compVal!='0'){	
					for (var j in fieldIdComponents2) {
						var comp2=fieldIdComponents2[j];
						var compId2=comp2.id;
						
						if(compId2!=undefined){
							var compVal2=comp2.value;
							
							var sensorName2=comp2.getAttribute(SENSOR_NAME_ATTR);
							var itemName2=comp2.getAttribute(ITEM_NAME_ATTR);
							
							if(compId!=compId2 && sensorName!=sensorName2 && itemName!=itemName2 && compVal==compVal2){
								
								var sensor=getCurrentItemFromCurrentJson('name',sensorName2);
								var item=getCurrentItemFromParentItem(sensor,'items','name',itemName2);
								
								if(sensor!=undefined && item!=undefined){
									result+=' Значение ="'+compVal2+'" уже используется: '+' Устройство "'+sensor.descr+'"'+'Датчик "'+item.descr+'"; <br>';
								}
																
								markComponentValidity(comp,false);
								markComponentValidity(comp2,false);
								
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