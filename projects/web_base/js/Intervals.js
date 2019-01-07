/*-----------------------Sensors values processing--------------------------------*/
function putIntervalContentToContainer(container,sensor,noId,editable){
	var sensorId=sensor.id;
	var sensorName=sensor.name;
	var sensorType=sensor.type;
	var sensorSize=sensor.size;
	var sensorDescr=sensor.descr;
		
	var hr = document.createElement('hr');
	container.appendChild(hr);
	container.appendChild(hr);
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
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 44px;');
	
	var col1h=createDivComponent('w3-quarter','');
	var col2h=createDivComponent('w3-quarter','');
	var col3h=createDivComponent('w3-quarter','');
	var col4h=createDivComponent('w3-quarter','');
	
	var editButton=createItemEditButton(sensorName,sensorDescr);
	var sName=createHeaderElement('H4','margin-left: 20px;',sensorName);
	var sType=createHeaderElement('H4','margin-left: 20px;',sensorType);
	var sSize=createHeaderElement('H4','margin-left: 20px;',sensorSize);
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
		
	}
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function validateCurrentIntervalForm(){
	var errorMessage='';
	var validateForm=getComponentById(currentItemPreffix+'_form');
		
	errorMessage=errorMessage+validateFieldValuesUnique(validateForm,QUEUE_SUFFIX,"Неверное значение 'Очередь'; <br>","Поле 'Очередь' не уникальное; <br>",false);
	errorMessage=errorMessage+validateFieldValuesUnique(validateForm,FIELD_ID_SUFFIX,"Неверное значение 'Поле ThingSpeak'; <br>","'Поле ThingSpeak' не уникальное; <br>",true);
	errorMessage=errorMessage+validateMinMaxValues();
	errorMessage=errorMessage+validateDescrValues();
	
	return errorMessage;
}

function submitCurrentIntervalForm(){
	var form=getComponentById(currentItemPreffix+'_form');
	
	showMessage(currentMessageComp,'Сохраняю настройки интервала...','w3-yellow');
	
	postFormAsJsonVal(form,submitValuesUrl,validateCurrentIntervalForm,getCurrentItemHandler);
}

function validateSensorsPage(){
	var errorMessage='';
	
	
	return errorMessage;
}