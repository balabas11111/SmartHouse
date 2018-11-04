//------------------------------Base logic----------------------------
function onLoadPageComplete(){
	w3_close();
	 
}
//----------------------------------------------------------------------
const DESCR_SUFFIX="descr";
const MIN_VAL_SUFFIX="minVal";
const MAX_VAL_SUFFIX="maxVal";
const FIELD_ID_SUFFIX="fieldId";

const SENSOR_NAME_ATTR='sensorName';
const ITEM_NAME_ATTR='itemName';

const SENSORS="sensors_";

var currentTab='';
var currentContainerName='';

function openTab(tabName,headerName) {
	currentTab=tabName;
	
	var headerComponent=document.getElementById('settingsHeaderName');
	
	if(headerComponent!=undefined){
		headerComponent.innerHTML=headerName;
	}
	
    var i;
    var x = document.getElementsByClassName("settingsTab");
    for (i = 0; i < x.length; i++) {
       x[i].style.display = "none";  
    }
    document.getElementById(tabName).style.display = "block"; 

	var containerComponent=document.getElementById(tabName);
	
	if(containerComponent.classList.contains('reloadableSettingsContainer')){
		currentContainerName=tabName+'_content';
		
		document.getElementById(currentContainerName).innerHTML="Загружаю данные...";
		
		if(tabName=='sensors'){
			updateComponentsByAjaxCall('GET', '/getAllSensorsJson', processSensorsJsonGet,"", 0);
		}
	}
}
//-------------------------------------------------------
function processSensorsJsonGet(data){
	var container=document.getElementById(currentContainerName);

	container.innerHTML='';
	var sensors=data.sensors;
			
	for(var s in sensors){
		var sensorName=sensors[s].name;
		var sensorDescr=sensors[s].descr;
		
		var sensorsHeaderText=sensorName+' ('+sensorDescr+')';
				
		var header = document.createElement("h2");
		header.setAttribute('style','margin-left: 20px;');
		var textNode = document.createTextNode(sensorsHeaderText);
		header.appendChild(textNode);
		container.appendChild(header);
		
		var hr = document.createElement('hr');
		container.appendChild(hr);
		
		var div=document.createElement('div');
		div.setAttribute('id',sensorName+'_div');
		div.setAttribute('class','w3-row');
		div.setAttribute('style','margin-top: 15px;');
		
		var div1=document.createElement('div');
		div1.setAttribute('class','w3-half');
		
		var div2=document.createElement('div');
		div2.setAttribute('class','w3-half');
				
		var col1=document.createElement('div');
		col1.setAttribute('class','w3-half w3-green');
		col1.innerHTML='Тип датчика';
		
		var col2=document.createElement('div');
		col2.setAttribute('class','w3-half w3-green');
		col2.innerHTML='Описание';
		
		var col3=document.createElement('div');
		col3.setAttribute('class','w3-third w3-green');
		col3.innerHTML='Мин значение';
		
		var col4=document.createElement('div');
		col4.setAttribute('class','w3-third w3-green');
		col4.innerHTML='Макс значение';
		
		var col5=document.createElement('div');
		col5.setAttribute('class','w3-third w3-green');
		col5.innerHTML='Поле ThingSpeak';
				
		div1.appendChild(col1);
		div1.appendChild(col2);
		div2.appendChild(col3);
		div2.appendChild(col4);
		div2.appendChild(col5);
		
		div.appendChild(div1);
		div.appendChild(div2);
		container.appendChild(div);
		
		var items=sensors[s].items;
		
		for(var i in items){
			var itemName=items[i].name;
			var descr=items[i].descr;
			var minVal=items[i].minVal;
			var maxVal=items[i].maxVal;
			var fieldId=items[i].fieldId;
			
					var div=document.createElement('div');
					div.setAttribute('class','w3-row');
					div.setAttribute('style','margin-top: 5px;');
					
					var div1=document.createElement('div');
					div1.setAttribute('class','w3-half');
					
					var div2=document.createElement('div');
					div2.setAttribute('class','w3-half');
					
					var col1=document.createElement('div');
					col1.setAttribute('class','w3-half');
					//col1.setAttribute("style",'margin-left: 10px; margin-right: 10px;');
										
					var col1Child=document.createElement('div');
					col1Child.setAttribute('class','w3-half');
					col1Child.setAttribute('style','margin-left: 20px; width: 90%;');
					col1Child.innerHTML="<b>"+itemName+"</b>";
					col1Child.setAttribute('id',"sensors_"+sensorName+"_"+itemName);
					col1Child.setAttribute(SENSOR_NAME_ATTR,sensorName);
					col1Child.setAttribute(ITEM_NAME_ATTR,itemName);
					
					var col2=document.createElement('div');
					col2.setAttribute('class','w3-half');
					//col2.setAttribute("style",'margin-left: 10px; margin-right: 10px;');
										
					var col3=document.createElement('div');
					col3.setAttribute('class','w3-third');
					//col3.setAttribute("style",'margin-left: 10px; margin-right: 10px;');
									
					var col4=document.createElement('div');
					col4.setAttribute('class','w3-third');
					//col4.setAttribute("style",'margin-left: 10px; margin-right: 10px;');
										
					var col5=document.createElement('div');
					col5.setAttribute('class','w3-third');
					//col5.setAttribute("style",'margin-left: 10px; margin-right: 10px;');
									
					var input2=document.createElement("Input");
					input2.setAttribute("id",getInputCompName(sensorName,itemName,DESCR_SUFFIX));
					input2.setAttribute("class","w3-input w3-border "+DESCR_SUFFIX);
					input2.setAttribute("style",'width: 90%;');
					input2.setAttribute("value",descr);
					input2.setAttribute(SENSOR_NAME_ATTR,sensorName);
					input2.setAttribute(ITEM_NAME_ATTR,itemName);
									
					var input3=document.createElement("Input");
					input3.setAttribute("id",getInputCompName(sensorName,itemName,MIN_VAL_SUFFIX));
					input3.setAttribute("class","w3-input w3-border "+MIN_VAL_SUFFIX);
					input3.setAttribute("style",'width: 90%;');
					input3.setAttribute("value",minVal);
					input3.setAttribute(SENSOR_NAME_ATTR,sensorName);
					input3.setAttribute(ITEM_NAME_ATTR,itemName);
										
					var input4=document.createElement("Input");
					input4.setAttribute("id",getInputCompName(sensorName,itemName,MAX_VAL_SUFFIX));
					input4.setAttribute("class","w3-input w3-border "+MAX_VAL_SUFFIX);
					input4.setAttribute("style",'width: 90%;');
					input4.setAttribute("value",maxVal);
					input4.setAttribute(SENSOR_NAME_ATTR,sensorName);
					input4.setAttribute(ITEM_NAME_ATTR,itemName);
					
					var input5=document.createElement("Input");
					input5.setAttribute("id",getInputCompName(sensorName,itemName,FIELD_ID_SUFFIX));
					input5.setAttribute("class","w3-input w3-border "+FIELD_ID_SUFFIX);
					input5.setAttribute("style",'width: 90%;');
					input5.setAttribute("value",fieldId);
					input5.setAttribute(SENSOR_NAME_ATTR,sensorName);
					input5.setAttribute(ITEM_NAME_ATTR,itemName);
					
					col1.appendChild(col1Child);
					
					col2.appendChild(input2);
					col3.appendChild(input3);
					col4.appendChild(input4);
					col5.appendChild(input5);
										
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
				
		var hr = document.createElement('hr');
		container.appendChild(hr);
		//end of sensors loop
	}	
	
		var p=document.createElement("p");
		var butt=document.createElement("button");
		butt.setAttribute("class","w3-btn w3-teal");
		butt.setAttribute("onclick","submitSensorsForm('sensors_form','sensors_msg');");
		butt.innerHTML="Сохранить";
		
		p.appendChild(butt);
		
		container.appendChild(p);
}

function getInputCompName(sensorName,itemName,suffix){
	return SENSORS+sensorName+"_"+itemName+"_"+suffix;
}

function submitSensorsForm(formName,updateName){
	console.log('submitting form');
	
	var f = document.forms[formName];
	var isValidForm = f.checkValidity();
	
	var errorMessage='';
	
	if(isValidForm){
		errorMessage=errorMessage+validateFieldIdValues();
		errorMessage=errorMessage+validateMinMaxValues();
		errorMessage=errorMessage+validateDescrValues();
		
		if(errorMessage!=''){
			isValidForm=false;
		}
	}
	
	if(isValidForm){
		document.getElementById(currentContainerName).innerHTML="Сохраняю данные...";
		
		f.addEventListener('submit', function(evt){evt.preventDefault();},false);
		var formData = new FormData(f);
		formData.append("currentTab", currentTab);
				
		var request = new XMLHttpRequest();
		request.open("POST", "setup", true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					if (this.status == 200){
						
						var json = JSON.parse(this.responseText);
						processSensorsJsonGet(json);
						
					} else {
						document.getElementById(updateName).innerHTML = 'Йа ошиБко!';
					};
				};
			};
		request.send(formData);
	}else{
		errorMessage='Некоторые значения неверны:  '+errorMessage;
			
		document.getElementById('sensors_msg').innerHTML =errorMessage;
	}
}

function validateMinMaxValues(){
	var minValComponents=document.getElementsByClassName(MIN_VAL_SUFFIX);
		
	for (var i in minValComponents) {
		var comp=minValComponents[i];
		var compId=comp.id;
		var compVal=comp.value;
		
		var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
		var itemName=comp.getAttribute(ITEM_NAME_ATTR);
		
		var maxComp=getMaxComponent(sensorName,itemName);
		var maxCompVal=maxComp.value;
		
		if(compVal>=maxCompVal){
			result+="Сенсор "+sensorName+" Тип датчика "+itemName+" неверное значение 'Мин/Макс' мин должно быть меньше макс; <br>";
		}
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
	
	for (var i in descrComponents) {
		var comp=descrComponents[i];
		var compId=comp.id;
		var compVal=comp.value;
				
		if(compVal==''){
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
			
			result+="Сенсор "+sensorName+" Тип датчика "+itemName+" пустое значение 'Описание'; <br>";
		}
	}

	return result;
}

function validateFieldIdValues(){
	var result='';
	var fieldIdComponents=document.getElementsByClassName(FIELD_ID_SUFFIX);
	var fieldIdComponents2=document.getElementsByClassName(FIELD_ID_SUFFIX);
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		var compId=comp.id;
		var compVal=comp.value;
				
		if(compVal=='' || compVal=='0' || compVal<0){
			
			var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
			var itemName=comp.getAttribute(ITEM_NAME_ATTR);
			
			result+="Сенсор "+sensorName+" Тип датчика "+itemName+" неверное значение 'Поле ThingSpeak'; <br>";
		}else{
			for (var j in fieldIdComponents2) {
				var comp2=fieldIdComponents2[j];
				var compId2=comp2.id;
				var compVal2=comp2.value;
				
				if(compId!=compId2 && compVal==compVal2){
					var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
					var itemName=comp.getAttribute(ITEM_NAME_ATTR);
					
					var sensorName2=comp2.getAttribute(SENSOR_NAME_ATTR);
					var itemName2=comp2.getAttribute(ITEM_NAME_ATTR);
					
					result+="Сенсор "+sensorName+" Тип датчика "+itemName+" 'Поле ThingSpeak' не уникально --> сенсор "+sensorName2+" тип датчика "+itemName2+"; <br>";
				}
				
			}
		}
	}

	return result;
}