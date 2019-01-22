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
function showDeleteIntervalForm(id){
	var message='Подтвердите удаление интервала '+document.getElementById('intervals_name_'+id).value;
	
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
	var countDiv=document.getElementById('intervals_count');
	var templateName='intervals-template';
	
	var intervals=data.intervals;
	var count=data.itemCount;
	
	countDiv.innerHTML=(count>0)?('Всего интервалов '+count):'Не задано ни одного интервала';
	container.innerHTML='';
			
	for(var i in intervals){
		var interval=intervals[i];
		var suffix='_'+interval.id;
		
		interval.daysArr=interval.days.split(',');
		putIntervalToForm(templateName,container,interval,true,suffix);
	}
	
	var msg=(customOnGetMessage!=undefined)?customOnGetMessage:('Загружено '+currentHeaderName);
	
	showStatusMessage(msg,'w3-green');
	customOnGetMessage=undefined;
}
function showNewIntervalForm(){
	var dt=new Date();
	var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
	
	var interval={};
	
	interval.id=-1;
	interval.name='Будильник';
	interval.type=0;
	interval.state=0;
	interval.startTime=tmp+60;
	interval.endTime=tmp+120;
	interval.time='0';
	interval.days='0,0,0,0,0,0,0';
	interval.daysArr=interval.days.split(',');
	interval.kind=0;
	
	var header='➲ Создание нового интервала';
	openIntervalPopup(interval,header);
}
function showEditIntervalForm(id){

	var ids=['id','name','type','state','startTime','endTime','time','kind'];
	
	var interval={};
	interval.daysArr=[];
	
	for(var i=0;i<ids.length;i++){
		interval[ids[i]]=getComponentValue(document.getElementById('intervals_'+ids[i]+'_'+id));
	}
	for(var i=0;i<7;i++){
		interval.daysArr[i]=getComponentValue(document.getElementById('intervals_days_'+i+'_'+id));
	}
		
	var header='➲ Редактирование : '+interval.name;
	openIntervalPopup(interval,header);
}
function openIntervalPopup(interval,header){
	var suffix='';
	var templateName=(interval.id>0)?'intervals_'+interval.id:'intervals-template';
	
	var headComp=getComponentById('intervals_cur_header');
	var container=getComponentById('intervals_cur_content');
	var msgComp=getComponentById('intervals_cur_msg');
	
	headComp.innerHTML=header;
	container.innerHTML='';
	
	msgComp.innerHTML='';
	setStatusMessageComp(msgComp);
	
	putIntervalToForm(templateName,container,interval,false,suffix);
	
	showComponent('intervals_cur_modal');
}
function putIntervalToForm(templateName,container,interval,disabled,suffix){
	var template=document.getElementById(templateName).content.cloneNode(true);
	
	template.querySelector('div.intervals').id = 'intervals'+suffix;
	
	if(!disabled){
		var editBtn=template.querySelector('div.editBtn');
		var delBtn=template.querySelector('div.deleteBtn');
		
		editBtn.parentNode.removeChild(editBtn);
		delBtn.parentNode.removeChild(delBtn);
		
	}else{	
		template.querySelector('span.editBtn').innerText = '➲ Редактирование '+interval.name;
		template.querySelector('span.deleteBtn').innerText = '➲ Удаление '+interval.name;
		
		template.querySelector('button.editBtn').setAttribute('onclick',"showEditIntervalForm("+interval.id+");");
		template.querySelector('button.deleteBtn').setAttribute('onclick',"showDeleteIntervalForm("+interval.id+");");
	}
	
	var fields=['id','name','type','state','startTime','endTime','time','kind'];
	
	for(var i=0;i<fields.length;i++){
		updateIntervalTemplate(template,fields[i],interval[fields[i]],suffix,disabled);
	}
	
	for(var i=0;i<7;i++){
		updateIntervalTemplate(template,'days_'+i,interval.daysArr[i],suffix,disabled);
	}
	
	var daysDiv=template.querySelector('div.days');
	var timeDiv=template.querySelector('div.time');
	var kindDiv=template.querySelector('div.kind');
	
	daysDiv.id='intervals_daysDiv'+suffix;
	timeDiv.id='intervals_timeDiv'+suffix;
	kindDiv.id='intervals_kindDiv'+suffix;
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,intervalType);

	container.appendChild(template);
}
function updateIntervalTemplate(template,field,value,suffix,disabled){
	var comp=template.querySelector('.intervals.'+field);
	comp.id='intervals_'+field+suffix;
	comp.disabled=disabled;
	setComponentValue(comp,value);
	
	var lbl=template.querySelector('label.'+field);
	if(lbl!=undefined){lbl.htmlFor=comp.id;}
}
function handlePeriodTypeChange(selectedIndex){
	var daysDiv=getComponentById('intervals_daysDiv');
	var timeDiv=getComponentById('intervals_timeDiv');
	var kindDiv=getComponentById('intervals_kindDiv');
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,kindDiv,selectedIndex);
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
	var form=document.getElementById('intervals_cur_form');
	postForm(form,submitValuesUrl,validateValuesHandler,constructIntervalsFormDataAsJson,validateStatusMessageDefault,onIntervalSavedHandler,getStatusMessageComp());
}
function validateCurrentIntervalForm(){
	var form=document.getElementById('intervals_cur_form');
	
	markFormAsValid(form,getStatusMessageComp(),true,true);
	markComponentValidityById('days',true);
	
	var errorMessage='';
	
	var idInput=document.getElementById('intervals_id');
	var nameInput=document.getElementById('intervals_name');
	var typeSelect=document.getElementById('intervals_type');
	var stateSelect=document.getElementById('intervals_state');
	var startTimeInput=document.getElementById('intervals_startTime');
	var endTimeInput=document.getElementById('intervals_endTime');
	
	var daysDiv=document.getElementById('intervals_days');
	var timeInput=document.getElementById('intervals_time');
	var kindSelect=document.getElementById('intervals_kind');
	
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
					&& nameComponents[i].id!='intervals_name_'+idInput.value
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
function constructIntervalsFormDataAsJson(form){
	var formInputs = getComponentChildrenByClass(form,'intervals');
	
	var json='{';
	var pageName='';
	
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
	
	var days='';
	for(var i=0;i<7;i++){
		days+=getComponentValue(document.getElementById('intervals_days_'+i))+',';
	}
	
	if(days.substring(days.length - 1)==','){
		days=days.substring(0,days.length-1);
	}
	
	json=json+'"days": "'+days+'"';
	
	if(json.substring(json.length - 1)==','){
		json=json.substring(0,json.length-1);
	}
	
	json+='}';
	
	var target=document.getElementById('intervals_target').value;
	
	return constructFormData_JSONprocessor(target,currentTab,json);
}
function onIntervalSavedHandler(data){
	var ok=isStatusMessageResponseOk(data.statusHttp);
	
	if(ok){
		hideComponent('intervals_cur_modal');
		reloadCurrentSettingsTab();
		showStatusMessage('Сохранено. Обновляю данные','w3-yellow');
	}else{
		var msg='Ошибка '+data.message;
		showStatusMessage(msg,'w3-red');
		 throw msg; 
	}
}