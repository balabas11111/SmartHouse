/*-----------------------Sensors values processing--------------------------------*/
function putSensorContentToContainer(container,sensor,noId,editable){
	if(sensor==undefined || sensor.kind==undefined
			|| (sensor.kind!='KIND_PINDIGITAL' && sensor.kind!='KIND_SENSOR')){
		return;
	}
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
	
	var div1h=createDivComponent('','w3-quarter','','Название устройства');
	var div2h=createDivComponent('','w3-quarter','','Тип устр.');
	var div3h=createDivComponent('','w3-quarter','','Измеряемые величины');
	var div4h=createDivComponent('','w3-quarter','','Описание устройства');
	
	divh.appendChild(div1h);
	divh.appendChild(div2h);
	divh.appendChild(div3h);
	divh.appendChild(div4h);
	
	container.appendChild(divh);
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 44px;');
	
	var col1h=createDivComponent('','w3-quarter');
	var col2h=createDivComponent('','w3-quarter');
	var col3h=createDivComponent('','w3-quarter');
	var col4h=createDivComponent('','w3-quarter');
	
	
	
	var sName=createHeaderElement('H4','margin-left: 20px;',sensorName);
	var sType=createHeaderElement('H4','margin-left: 20px;',sensorType);
	var sSize=createHeaderElement('H4','margin-left: 20px;',sensorSize);
	var sDescr=createInputComponent(sensorId,255,FIELD_DESCR_ID,'id',sensorDescr,sensorName,itemName,noId,editable);
	
	if(editable==undefined || !editable){
		var cellDivButton=createDivComponent('','w3-cell');
		var editButton=createItemEditButton('name',sensorName,sensorDescr,undefined);
		
		cellDivButton.appendChild(editButton);
		
		var cellDivName=createDivComponent('','w3-cell w3-container');
		cellDivName.appendChild(sName);
		
		col1h.appendChild(cellDivButton);
		col1h.appendChild(cellDivName);
	}else{
		col1h.appendChild(sName);
	}
	
	col2h.appendChild(sType);
	col3h.appendChild(sSize);
	col4h.appendChild(sDescr);
					
	divhv.appendChild(col1h);
	divhv.appendChild(col2h);
	divhv.appendChild(col3h);
	divhv.appendChild(col4h);
	
	container.appendChild(divhv);
	container.appendChild(hr);
	
	if(!(sensor.items==undefined || sensor.items.length==0)){
	
		var div=document.createElement('div');
		div.setAttribute('id',sensorName+'_div');
		div.setAttribute('class','w3-row w3-teal');
		div.setAttribute('style','margin-top: 2px;');
		
		var div1=createDivComponent('','w3-threequarter');
		var div2=createDivComponent('','w3-quarter');
		
		var col0=createDivComponent('','w3-third','',' Датчики устройства');
		var col1=createDivComponent('','w3-third','','Описание');
		var col2=createDivComponent('','w3-third','','Очередь');
		
		var col3=createDivComponent('','w3-third','','Миним. значение');
		var col4=createDivComponent('','w3-third','','Макс значение');
		var col5=createDivComponent('','w3-third','','Поле ThingSpeak');
		
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
				
				var div1=createDivComponent('','w3-threequarter');
				var div2=createDivComponent('','w3-quarter');
				
				var col0=createDivComponent('','w3-third');
				var col1=createDivComponent('','w3-third');
				var col2=createDivComponent('','w3-third');
									
				var col3=createDivComponent('','w3-third');
				var col4=createDivComponent('','w3-third');
				var col5=createDivComponent('','w3-third');
				
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
	}
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function validateCurrentSensorForm(){
	var errorMessage='';
		
	errorMessage=errorMessage+validateFieldValuesUnique(currentForm,QUEUE_SUFFIX,"Неверное значение 'Очередь'; <br>","Поле 'Очередь' не уникальное; <br>",false);
	errorMessage=errorMessage+validateFieldValuesUnique(currentForm,FIELD_ID_SUFFIX,"Неверное значение 'Поле ThingSpeak'; <br>","'Поле ThingSpeak' не уникальное; <br>",true);
	errorMessage=errorMessage+validateMinMaxValues();
	errorMessage=errorMessage+validateDescrValues();
	
	return errorMessage;
}

/*------------------------validators------------------------------------*/
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