/*-----------------------Sensors values processing--------------------------------*/
var rescheduleIndex=-1;
var multidailyIndex=-1;
var periodicIndex=-1;
var intervalType=[];
var intervalState=[];
var dayOfWeekShort=[];
var dayOfWeek=[];
var intervalKind=[];

function fillParameters(data){
	fillParameterArray(intervalType,data.intervalType);
	fillParameterArray(intervalState,data.intervalState);
	fillParameterArray(dayOfWeekShort,data.dayOfWeekShort);
	fillParameterArray(dayOfWeek,data.dayOfWeek);
	fillParameterArray(intervalKind,data.intervalKind);
	
	multidailyIndex=data.multidailyIndex;
	periodicIndex=data.periodicIndex;
	rescheduleIndex=data.rescheduleIndex;
}

function fillParameterArray(targetArray,jsonArray){
	if(targetArray.length==0){
		for(var i=0;i<jsonArray.length;i++){targetArray[i]=jsonArray[i];}
	}
}

function openNewIntervalForm(){
	var dt=new Date();
	var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
	
	var interval={};
		interval.id=-1;
		interval.name='Новый';
		interval.type=0;
		interval.state=0;
		interval.startTime=tmp+60;
		interval.endTime=tmp+120;
		interval.time='0';
		interval.days='0,0,0,0,0,0,0';
		interval.kind=0;
		
		openItemPopup('','','Новый интервал',interval);
}

function openConfirmDeleteIntervalForm(tag,id,name){
	var message='Подтвердите удаление интервала '+name;
	
	var btnNames=['Удалить','Отменить'];
	var btnToolTips=['',''];
	var btnClasses=['w3-border-red','w3-border-red'];
	var btnOnClicks=['confirmDeleteInterval('+id+');','confirmDlgHide();'];
	
	confirmDlgInit('Подтверждение',message,'',undefined,undefined,btnNames,btnToolTips,btnClasses,btnOnClicks,false);
	confirmDlgShow();
}

function confirmDeleteInterval(id){
	showStatusMessage('Удаляю интервал ...','w3-yellow');
	
    var target = createHiddenInput(undefined,'remote_target','timeIntervalService');
    var page = createHiddenInput(undefined,'remote_page','delete');
    var val = createHiddenInput(undefined,'val_json',id);

    var form = document.createElement("form");
    form.appendChild(target);
    form.appendChild(page);
    form.appendChild(val);
	
	postForm(form,submitValuesUrl,undefined,undefined,validateStatusMessageDefault,onIntervalDeletedHandler,getStatusMessageComp());
}

function onIntervalDeletedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		confirmDlgHide();
		reloadCurrentSettingsTab();
		showStatusMessage('Удалено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function processIntervalsJsonGet(data){
	fillParameters(data);
	var container=document.getElementById('intervals_form');

	container.innerHTML='';
	var items=data.intervals;
	
	var count=data.itemCount;
	
	document.getElementById('intervalsCountDiv').innerHTML=(count>0)?('Всего интервалов '+count):'Не задано ни одного интервала';
			
	for(var s in items){
		var item=items[s];
		putIntervalToForm(container,item,false);
	}
	
	var msg=(customOnGetMessage!=undefined)?customOnGetMessage:('Загружено '+currentHeaderName);
	
	showStatusMessage(msg,'w3-green');
	customOnGetMessage=undefined;
}

var intTmpl=undefined;
var intIdSuf=undefined;

function putIntervalToForm(container,interval,editable){
	intTmpl = document.getElementById('intervals-template').content.cloneNode(true);
	
	if(!editable){
		var btnTmpl = document.getElementById('intervals-template').content.cloneNode(true);
		
		btnTmpl.querySelector('.editTooltip').innerText = '➲ Редактирование '+interval.name;
		btnTmpl.querySelector('.editBtn').setAttribute('onclick',"openIntervalEditPopup('"+interval.id+"','➲ Редактирование "+interval.name+"');");
		btnTmpl.querySelector('.deleteTooltip').innerText = '➲ Редактирование '+interval.name;
		btnTmpl.querySelector('.deleteBtn').setAttribute('onclick',"openIntervalDeletePopup("+interval.id+",'✂ Подтверждение');");
		
		intTmpl.querySelector('div.id').appendChild(btnTmpl);
	}
	
	intIdSuf=(editable)?'':'_'+interval.id;
	
	var id=getIntEl('id',interval.id);
	var name=getIntEl('name',interval.name);
	var type=getIntEl('type',interval.type);
	var state=getIntEl('state',interval.state);
	var startTime=getIntEl('startTime',interval.startTime);
	var endTime=getIntEl('endTime',interval.endTime);
	var time=getIntEl('time',interval.time);
	var kind=getIntEl('kind',interval.kind);
	
	var days=interval.days.split(',');
	
	for(var i=0;i<7;i++){
		getIntEl('days_'+i,days[i]);
	}

	container.appendChild(intTmpl);
}

function getIntEl(id,value){
	var sel='.intervals.'+id;
	var comp=intTmpl.querySelector(sel);
	comp.id=id+intIdSuf;
	setComponentValue(comp,value);
	
	var lblSel='label.'+id;
	var lbl=intTmpl.querySelector(lblSel);
	if(lbl!=undefined){lbl.htmlFor=comp.id;}
	return comp;
}

function handlePeriodTypeChange(selectedIndex){
	var daysDiv=getComponentById('daysDiv');
	var timeDiv=getComponentById('timeDiv');
	var kindDiv=getComponentById('kindDiv');
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,selectedIndex)
}

/*show or hide days interval component based on type*/
function applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,typeInt){
	var isMultidaily=(typeInt==multidailyIndex);
	var isPeriodic=(typeInt==periodicIndex);
	var isKindVis=true;
	
	setVisible(daysDiv,isMultidaily);
	setVisible(timeDiv,isPeriodic);
	setVisible(kindDiv,isKindVis);
	
	markFormAsValid(currentForm,getStatusMessageComp(),true,true);
}

function submitIntervalsFormAsJsonReloadCurrTab(){
	showStatusMessage('Сохраняю интервалы ...','w3-yellow');
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructIntervalsFormDataAsJson,validateStatusMessageDefault,onIntervalSavedHandler,getStatusMessageComp());
}

function onIntervalSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		hideComponent(currentItemPreffix+'_modal');
		reloadCurrentSettingsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}

function constructIntervalsFormDataAsJson(form){
	
	var formInputs = getComponentChildrenByClass(form,'intervals');
	
	var json='{';
	var pageName='';
	
	var days={};
	
	for(var i=0;i<formInputs.length;i++){
		var child=formInputs[i];
		if(child!=undefined && child.tagName!=undefined && child.type!=undefined
				&& child.name!=undefined && child.id!=undefined){

				if(child.name.substring(0,8)!='cb_days_'){
				
					inputName=child.name;
					inputValue=getComponentValue(child);
				
					json=json+'"'+inputName+'": "'+inputValue+'",';
				}
		}
	}
	
	json=json+'"days": "'+checkBoxListToString('days')+'"';
	
	if(json.substring(json.length - 1)==','){
		json=json.substring(0,json.length-1);
	}
	
	json+='}';
	
	var targetId=form.getAttribute('id')+'_target';
	var target=getComponentValueById(targetId);
	
	return constructFormData_JSONprocessor(target,currentTab,json);
}

function validateCurrentIntervalForm(){
	markFormAsValid(currentForm,getStatusMessageComp(),true,true);
	markComponentValidityById('days',true);
	
	var errorMessage='';
	
	var idInput=getComponentById('id');
	var nameInput=getComponentById('name');
	var typeSelect=getComponentById('typeInt');
	var stateSelect=getComponentById('stateInt');
	var startTimeInput=getComponentById('startTime');
	var endTimeInput=getComponentById('endTime');
	
	var daysDiv=getComponentById('days');
	var timeInput=getComponentById('time');
	var kindSelect=getComponentById('kind');
	
	if(nameInput==undefined 
			|| typeSelect==undefined || typeSelect.selectedIndex==undefined  
			|| stateSelect==undefined || stateSelect.selectedIndex==undefined 
			|| startTimeInput==undefined || endTimeInput==undefined 
			|| timeInput==undefined 
			|| kindSelect==undefined || kindSelect.selectedIndex==undefined){
		return 'Общая ошибка приложения <br>';
	}
	
	if(nameInput.value==undefined || nameInput.value==''){
		errorMessage+=markComponentValidityWithMessage(nameInput,false,'Пустое поле имя <br>');
	}else{
		var nameComponents=document.getElementById('intervals_form').getElementsByClassName('intervals');
		
		if(nameComponents!=undefined){
			for(var i=0;i<nameComponents.length;i++){
				if(nameComponents[i]!=undefined && nameComponents[i].name=='name'
					&& nameComponents[i].value!=undefined && nameComponents[i].id!=nameInput.id
					&& nameComponents[i].id!='name_'+idInput.value
					&& nameInput.value==nameComponents[i].value){
					
					errorMessage+=markComponentValidityWithMessage(nameInput,false,'Название не уникально <br>');
				}
			}
		}
	}
	
	var typeInt=typeSelect.selectedIndex;
	
	if(typeInt<0){
		errorMessage+=markComponentValidityWithMessage(typeSelect,false,'Пустое поле тип <br>');
	}
	
	if(stateSelect.selectedIndex<0){
		errorMessage+=markComponentValidityWithMessage(stateSelect,false,'Пустое поле состояние <br>');
	}
	
	var startTime=getComponentValue(startTimeInput);
	var endTime=getComponentValue(endTimeInput);
	
	if(startTime==undefined){
		errorMessage+=markComponentValidityWithMessage(startTimeInput,false,'Пустое поле Старт <br>');
	}
	
	if(endTime==undefined){
		errorMessage+=markComponentValidityWithMessage(endTimeInput,false,'Пустое поле Старт <br>');
	}
	
	if(startTime>=endTime){
		errorMessage+=markComponentValidityWithMessage(startTimeInput,false,'Время начала больше или равно времени завершения <br>');
		errorMessage+=markComponentValidityWithMessage(endTimeInput,false,'Время Завершения меньше времени начала <br>');
	}
	
	if(typeInt==multidailyIndex){
		var days=checkBoxListToString('days');
		
		if(days.length!=13 || !days.includes('1')){
			errorMessage+=markComponentValidityWithMessage(daysDiv,false,'Не указан ни один день <br>');
		}
	}
	
	if(typeInt==periodicIndex){
		var time=getComponentValue(timeInput);
		
		if(time==undefined || time=='' || time==0 || time=='0' || time<60){
			errorMessage+=markComponentValidityWithMessage(timeInput,false,'Минимально возможное значение периода 60 сек <br>');
		}
	}
	
	return errorMessage;
}
