/*-----------------------Sensors values processing--------------------------------*/
var innactiveIndex=-1;
var multidailyIndex=-1;
var intervalType=[];
var intervalState=[];
var dayOfWeekShort=[];
var dayOfWeek=[];

function processIntervalsJsonGet(data){
	fillParameters(data);
	processItemsJsonGet(data);
}

function fillParameters(data){
	if(intervalType.length==0){
		var arr=data.intervalType;
		for(var i=0;i<arr.length;i++){intervalType[i]=arr[i];}
	}
	if(intervalState.length==0){
		var arr=data.intervalState;
		for(var i=0;i<arr.length;i++){intervalState[i]=arr[i];}
	}
	if(dayOfWeekShort.length==0){
		var arr=data.dayOfWeekShort;
		for(var i=0;i<arr.length;i++){dayOfWeekShort[i]=arr[i];}
	}
	if(dayOfWeek.length==0){
		var arr=data.dayOfWeek;
		for(var i=0;i<arr.length;i++){dayOfWeek[i]=arr[i];}
	}
	multidailyIndex=data.multidailyIndex;
	innactiveIndex=data.innactiveIndex;
}

function putIntervalContentToContainer(container,interval,noId,editable){
	var id=interval.id;
	var name=interval.name;
	var typeInt=interval.typeInt;
	var stateInt=interval.stateInt;
	var startTime=interval.startTime;
	var endTime=interval.endTime;
	var days=interval.days.split(',');
	var param=interval.param;
	
	var hr = document.createElement('hr');
	container.appendChild(hr);
	container.appendChild(hr);
	var divh=document.createElement('div');
	divh.setAttribute('class','w3-row w3-teal');
	divh.setAttribute('style','margin-top: 15px; height: 50px;');
	
	var div1h=createDivComponent('w3-quarter','Интервал');
	var div2h=createDivComponent('w3-quarter','Тип/Состояние.');
	var div3h=createDivComponent('w3-quarter','Старт/Стоп');
	var div4h=createDivComponent('w3-quarter','Параметры');
	
	divh.appendChild(div1h);
	divh.appendChild(div2h);
	divh.appendChild(div3h);
	divh.appendChild(div4h);
	
	container.appendChild(divh);
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 110px;');
	
	var col1h=createDivComponent('w3-quarter','');
	var col2h=createDivComponent('w3-quarter','');
	var col3h=createDivComponent('w3-quarter','');
	var col4h=createDivComponent('w3-quarter','');
	/* column 1*/
	
	var suf='';
	
	if(editable==undefined || !editable){
		suf='_'+id;
	}
	
	var cid='id'+suf;
	var cname='name'+suf;
	var ctypeInt='typeInt'+suf;
	var cstateInt='stateInt'+suf;
	var cstartTime='startTime'+suf;
	var cendTime='endTime'+suf;
	var cdays='days'+suf;
	var cparam='param'+suf;
	
	var inputId=createInputSimple(cid,'id',undefined,'',id,editable);
	var inputName=createInputSimple(cname,'name',undefined,'',name,editable);
	
	if(editable==undefined || !editable){
		var cellDivButton=createDivComponent('w3-cell','');
		var editButton=createItemEditButton('id',id,name);
		
		cellDivButton.appendChild(editButton);
		col1h.appendChild(cellDivButton);
	}
	
	var cellDivId=createDivComponent('w3-cell w3-container','');
	cellDivId.appendChild(inputId);
		
	var cellDivName=createDivComponent('','');
	cellDivName.appendChild(inputName);
		
	col1h.appendChild(cellDivId);
	col1h.appendChild(cellDivName);
	/* column 2*/
	
	var typeSelect=document.createElement("select");
	typeSelect.id=ctypeInt;
	typeSelect.name='typeInt';
	if(editable==undefined || !editable){typeSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(typeSelect,intervalType,typeInt);
	var cellDivType=createDivComponent('','');
	cellDivType.appendChild(typeSelect);
	
	var stateSelect=document.createElement("select");
	stateSelect.id=cstateInt;
	stateSelect.name='stateInt';
	if(editable==undefined || !editable){stateSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(stateSelect,intervalState,stateInt);
	var cellDivState=createDivComponent('','');
	cellDivState.appendChild(stateSelect);
	
	col2h.appendChild(cellDivType);
	col2h.appendChild(cellDivState);
	
	/* column 3*/
	var inputStartTime=createInputSimple(cstartTime,'startTime',undefined,'','',editable);
	inputStartTime.setAttribute('type','datetime-local');
	setComponentValue(inputStartTime,startTime);
	var cellDivStartTime=createDivComponent('','');
	cellDivStartTime.appendChild(inputStartTime);
	
	var inputendTime=createInputSimple(cendTime,'endTime',undefined,'','',editable);
	inputendTime.setAttribute('type','datetime-local');
	setComponentValue(inputendTime,endTime);
	var cellDivendTime=createDivComponent('','');
	cellDivendTime.appendChild(inputendTime);
	
	col3h.appendChild(cellDivStartTime);
	col3h.appendChild(cellDivendTime);
	
	/* column 4*/
	if(typeInt==multidailyIndex){
		var daysDiv=createDivComponent('','');
		daysDiv.id=cdays;
		daysDiv.name='days';
		
		arrayToCheckBoxList(daysDiv,cdays,days,dayOfWeekShort);
		
		col4h.appendChild(daysDiv);
	}
	
	var paramDiv=createDivComponent('','');
	var inputParam=createInputSimple(cparam,'param',undefined,'',param,editable);
	
	paramDiv.appendChild(inputParam);
	
	col4h.appendChild(paramDiv);
					
	divhv.appendChild(col1h);
	divhv.appendChild(col2h);
	divhv.appendChild(col3h);
	divhv.appendChild(col4h);
	
	container.appendChild(divhv);
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function validateCurrentIntervalForm(){
	var errorMessage='';
		
	
	return errorMessage;
}

