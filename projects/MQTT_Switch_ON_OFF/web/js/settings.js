//------------------------------Base logic----------------------------
function onLoadPageComplete(){
	document.title='Настройки устройства';
	w3_close();
}
//----------------------------------------------------------------------
const DESCR_SUFFIX="descr";
const MIN_VAL_SUFFIX="minVal";
const MAX_VAL_SUFFIX="maxVal";
const FIELD_ID_SUFFIX="fieldId";
const QUEUE_SUFFIX="queue";

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
var currentContainerName='';
var currentMessageCompName='';

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
	
	currentContainerName=tabName+'_content';
	currentMessageCompName=tabName+'_msg';
	document.title='Настройки устройства - '+tabName;
	
	if(containerComponent.classList.contains('reloadableSettingsContainer')){
				
		document.getElementById(currentContainerName).innerHTML="Загружаю данные...";
		
		if(tabName=='sensors'){
			var f = document.forms['sensors_form'];
			f.addEventListener('submit', 
								function(evt){
									evt.preventDefault();
								},false);
			
			updateComponentsByAjaxCall('GET', '/getAllSensorsJson', processSensorsJsonGet,"", 0);
		}
	}
}

//-------------------------------------------------------
function createDivComponent(className,innerHtml){
	var div=document.createElement('div');
		div.setAttribute('class',className);
		if(innerHtml!=undefined && innerHtml!=''){
			div.innerHTML=innerHtml;
		}
		
	return div;
}

function createInputComponent(id1,id2,fieldIntId,suffixName,fieldVal,sensorName,itemName){
	var input1=document.createElement("Input");
		input1.setAttribute("id",getInputCompName(id1,id2,fieldIntId));
		input1.setAttribute("name",getInputCompName(id1,id2,fieldIntId));
		input1.setAttribute("class","w3-input w3-border "+suffixName);
		input1.setAttribute("style",'width: 90%;');
		input1.setAttribute("value",fieldVal);
		input1.setAttribute(SENSOR_NAME_ATTR,sensorName);
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
//-------------------------------------------------------
function processSensorsJsonGet(data){
	var container=document.getElementById(currentContainerName);

	container.innerHTML='';
	var sensors=data.sensors;
			
	for(var s in sensors){
		var sensorId=sensors[s].id;
		var sensorName=sensors[s].name;
		var sensorType=sensors[s].type;
		var sensorSize=sensors[s].size;
		var sensorDescr=sensors[s].descr;
			
		var hr = document.createElement('hr');
		container.appendChild(hr);
		container.appendChild(hr);
		//sensor header 
		var divh=document.createElement('div');
		divh.setAttribute('class','w3-row w3-green');
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
				
		var sName=createHeaderElement('H4','margin-left: 20px;',sensorName)
		var sType=createHeaderElement('H4','margin-left: 20px;',sensorType)
		var sSize=createHeaderElement('H4','margin-left: 20px;',sensorSize)
		var sDescr=createInputComponent(sensorId,255,FIELD_DESCR_ID,DESCR_SUFFIX,sensorDescr,sensorName,itemName);
		
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
		div.setAttribute('class','w3-row w3-black');
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
		var items=sensors[s].items;
		
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

				var input1=createInputComponent(sensorId,itemId,FIELD_DESCR_ID,DESCR_SUFFIX,descr,sensorName,itemName);
				var input2=createInputComponent(sensorId,itemId,FIELD_QUEUE_ID,QUEUE_SUFFIX,queue,sensorName,itemName);
				var input3=createInputComponent(sensorId,itemId,FIELD_MIN_ID,MIN_VAL_SUFFIX,minVal,sensorName,itemName);
				var input4=createInputComponent(sensorId,itemId,FIELD_MAX_ID,MAX_VAL_SUFFIX,maxVal,sensorName,itemName);
				var input5=createInputComponent(sensorId,itemId,FIELD_FIELDID_ID,FIELD_ID_SUFFIX,fieldId,sensorName,itemName);
				
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
		//end of sensors loop
	}	
	
		var p=document.createElement("p");
		var butt=document.createElement("button");
		butt.setAttribute("class","w3-btn w3-teal");
		butt.setAttribute("style","margin-left: 20px; margin-bottom: 20px;");
		butt.setAttribute("onclick","submitSensorsForm('sensors_form','sensors_msg');");
		butt.innerHTML="Сохранить";
		
		p.appendChild(butt);
		
		container.appendChild(p);
}

function getInputCompName(sensorName,itemName,suffix){
	return SENSORS_SHORT+sensorName+"_"+itemName+"_"+suffix;
}

function submitSensorsForm(formName,updateName){
	console.log('submitting form');
	
	var f = document.forms[formName];
	var isValidForm = f.checkValidity();
	
	var errorMessage='';
	
	if(isValidForm){
		errorMessage=errorMessage+validateFieldValuesUnique(QUEUE_SUFFIX,"Неверное значение 'Очередь'; <br>","Поле 'Очередь' не уникальное; <br>",false);
		errorMessage=errorMessage+validateFieldValuesUnique(FIELD_ID_SUFFIX,"Неверное значение 'Поле ThingSpeak'; <br>","'Поле ThingSpeak' не уникальное; <br>",true);
		errorMessage=errorMessage+validateMinMaxValues();
		errorMessage=errorMessage+validateDescrValues();
		
		if(errorMessage!=''){
			isValidForm=false;
		}
	}
	
	if(isValidForm){
		
		var formData = new FormData(f);
		formData.append("currentTab", currentTab);
		
		showMessage('Сохраняю данные...','w3-yellow');
		
		var request = new XMLHttpRequest();
		request.open("POST", "/submitAllSensorsJson", true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					if (this.status == 200){
						
						var json = JSON.parse(this.responseText);
						processSensorsJsonGet(json);
						
						showMessage('Настройки сохранены!','w3-green');
						
					} else {
						showMessage('Ошибка на стророне сервера!','w3-red');
					};
				};
			};
		request.send(formData);
	}else{
		errorMessage='Некоторые значения неверны:  '+errorMessage;
		showMessage(errorMessage,'w3-red');
	}
}

function showMessage(message,className){
	var msgComp=document.getElementById(currentMessageCompName);
	
	if(className!=undefined && className!=''){
		msgComp.setAttribute('class',className);
	}
	
	if(message!=undefined && message!=''){
		msgComp.style.display = "block";
		msgComp.innerHTML = message;
	}else{
		msgComp.style.display = "none"; 
	}
}

function validateMinMaxValues(){
	var minValComponents=document.getElementsByClassName(MIN_VAL_SUFFIX);
	var maxValComponents=document.getElementsByClassName(MAX_VAL_SUFFIX);
	var result='';
	var error=false;
	
	for (var i in minValComponents) {
		var comp=minValComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
		}
	}
	
	for (var i in maxValComponents) {
		var comp=maxValComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
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
				comp.classList.add('w3-border-red');
				maxComp.classList.add('w3-border-red');	
				
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
			comp.classList.remove('w3-border-red');
		}
	}
	
	for (var i in descrComponents) {
		var comp=descrComponents[i];
		var compId=comp.id;
		if(compId!=undefined){
			var compVal=comp.value;
			
			if(compVal==''){
				comp.classList.add('w3-border-red');
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

function validateFieldValuesUnique(validClassName,errorMessage1,errorMessage2, notAllowEmpty){
	var result='';
	var error=false;
	var fieldIdComponents=document.getElementsByClassName(validClassName);
	var fieldIdComponents2=document.getElementsByClassName(validClassName);
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		if(comp!=undefined && comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
		}
	}
	
	for (var i in fieldIdComponents2) {
		var comp=fieldIdComponents2[i];
		if(comp!=undefined && comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
		}
	}
	
	for (var i in fieldIdComponents) {
		var comp=fieldIdComponents[i];
		var compId=comp.id;
		
		if(compId!=undefined){
			var compVal=comp.value;
					
			if((!compVal || 0 === compVal.length) || compVal<0){
				
				if(notAllowEmpty){
					var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
					var itemName=comp.getAttribute(ITEM_NAME_ATTR);
					
					comp.classList.add('w3-border-red');
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
							
							if(compId!=compId2 && compVal==compVal2){
								var sensorName=comp.getAttribute(SENSOR_NAME_ATTR);
								var itemName=comp.getAttribute(ITEM_NAME_ATTR);
								
								var sensorName2=comp2.getAttribute(SENSOR_NAME_ATTR);
								var itemName2=comp2.getAttribute(ITEM_NAME_ATTR);
								
								comp.classList.add('w3-border-red');
								comp2.classList.add('w3-border-red');
								
								error=true;
								result=errorMessage2;
							}
						
						}
						
					}
				}
			}
		}
	}
	
	return result;
}