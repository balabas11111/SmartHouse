@echo OFF

IF "%IP%"=="" ( 
    SET IP=192.168.0.120
)

echo ====================================================
echo                begin Validate Urls
echo                check IP=%IP% 
echo ====================================================

set RESULT_FILE=ipres.txt
set URL_FILE=urlres.txt

powershell -Command "Remove-Item %URL_FILE% -Force -Recurse -ErrorAction Ignore"

curl -Is %IP% > %RESULT_FILE% 

IF "%LOG_FILE%"=="" (
	SET LOG_FILE=%RESULT_FILE%
)

find /I "HTTP" %RESULT_FILE% >> %LOG_FILE%  && (
		echo IP returned response
	) || (
		echo ERROR ip is not available!!!
		GOTO:ERROR_LABEL
	)
	
set sources[0]=*"http://%IP%"
set sources[1]=*"http://%IP%/index"
set sources[2]=*"http://%IP%/settings"
set sources[3]=*"http://%IP%/edit"

set sources2[0]=*"http://%IP%/view?file=/settings/settings.txt"
set sources2[1]=*"http://%IP%/list?dir=/"
set sources2[2]=*"http://%IP%/espSettingsBox/getSimpleJson"
set sources2[3]=*"http://%IP%/gscv?sensor=all"
set sources2[4]=*"http://%IP%/getJson_settings?page=device"
set sources2[5]=*"http://%IP%/getJson_settings?page=net"
set sources2[6]=*"http://%IP%/getJson_sensors"
set sources2[7]=*"http://%IP%/getJson_intervals"

echo ====================================================  >> %URL_FILE%
for /F "tokens=2 delims=*" %%s in ('set sources2[') do (
	echo: >> %URL_FILE%
	echo %%s >> %URL_FILE%
	echo ==================================================== >> %URL_FILE%
	curl -X GET %%s -u balabas:balabas >> %URL_FILE%
	echo ==================================================== >> %URL_FILE%
)
echo ====================================================
pause
del "%RESULT_FILE%"
start notepad++ "%URL_FILE%"
exit

GOTO:EXIT_LABEL	
:ERROR_LABEL

echo ====================================================
echo        IP call result
echo ====================================================
type "%RESULT_FILE%"
echo: 
echo ====================================================
:EXIT_LABEL

del "%RESULT_FILE%"
