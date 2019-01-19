function submitManageCommand(){
	confirmDlgRemoveButtons();
	var action=confirmDlgGetGetInputValue('action');
	var remote_target=confirmDlgGetGetInputValue('remote_target');
	var remote_page=confirmDlgGetGetInputValue('remote_page');
	var val_json=confirmDlgGetGetInputValue('val_json');
	
	var form=getComponentById('confirm_form');
	var msg=getComponentById('confirm_msg');
	
	showMessage(msg,'Отправляю команду '+action+' на устройство...','w3-yellow');
	
	postForm(form,'/processJson?name='+remote_target,undefined,constructFormDataDefault,validateCommandManageResponse,processCommandManageResponse,msg);
}

function validateCommandManageResponse(resp){
	var json = JSON.parse(resp);
	
	var statusHttp=json.statusHttp;
	var message=json.message;
	var ok=isStatusMessageResponseOk(statusHttp);
	
	if(!ok){
		return message;
	}
	
	return "";
}

function processCommandManageResponse(resp){
	/*
	 {"statusInt": "14","statusCode": "Deleted","statusDescr": "Удалено","statusHttp": "202","message": "Удалено","data": ""}
	 */
	var action=confirmDlgGetGetInputValue('action');
	var statusHttp=resp.statusHttp;
	var message=resp.message;
	var data=resp.data;
	
	var ok=isStatusMessageResponseOk(statusHttp);
	
	if(ok){
		messageColorClass='w3-green';
	}else{
		messageColorClass='w3-red';
	}
	
	if(!ok){
		showStatusMessage(message,messageColorClass);
		return message;
	}
	
	if(action=='restart'){
		var messageColorClass='w3-yellow';
		confirmDlgRemoveButtons();
		confirmDlgCloseButtonVisible(false);
		
		confirmDlgSetHeader('Перезапуск');
		confirmDlgSetContent('Перезагрузка устройства');
		
		startRestart();
		
		return message;
	}
	if(action=='recreateChannels'){
		confirmDlgSetHeader('Сообщение системы');
		
		confirmDlgSetContent('Каналы и очереди ThingSpeak были пересозданы.<br> Перезапустите устройство для применения новых настроек.<br> Все предыдущие данные каналов находятся в старых каналах и могут быть просмотрены<br>');
		
		displayConfirmRestartDialog();
		
		return message;
	}
	
	if(action=='deleteSettings'){
		confirmDlgSetHeader('Сообщение системы');
		
		confirmDlgSetContent('Все настройки системы были удалены<br> Перезапустите устройство для применения настроек по умолчанию. <br>');
		
		displayConfirmRestartDialog();
		
		return message;
	}
}

function displayConfirmRestartDialog(){
	confirmDlgRemoveButtons();
	confirmDlgAddButton(0,'Закрыть и перезапустить','','w3-border-red','displayRestartDialog();');
	
	confirmDlgCloseButtonVisible(false);
					
	hideComponent('confirm_msg');
}
