@echo OFF

IF "%IP%"=="" ( 
    SET IP=192.168.0.120
)

echo                begin list files 
echo ====================================================
echo                check IP=%IP% 
echo ====================================================

set RESULT_FILE=ipres.txt
set FILES_FILE=files.txt

curl -Is %IP% > %RESULT_FILE% 

IF "%LOG_FILE%"=="" (
	SET LOG_FILE=%RESULT_FILE%
)

find /I "HTTP" %RESULT_FILE% >> %LOG_FILE%  && (
		echo IP returned response
	) || (
		echo ERROR ip is not available!!!
		GOTO:EXIT_LABEL
	)
	
SET URL=http://%IP%/list?dir=/%FOLDER%	

echo ====================================================
echo    URL    = %URL%
echo    output = %FILES_FILE% 
echo ====================================================
curl -s -X GET %URL% > %FILES_FILE%

setlocal EnableDelayedExpansion
set "cmd=findstr /R/N "^^" %FILES_FILE% | find /C ":""

for /f %%a in ('!cmd!') do set number=%%a

echo ====================================================
echo        %IP% files count = %number%
echo ====================================================
type "%FILES_FILE%"
echo ====================================================

powershell -Command "Remove-Item %LOG_FILE2% -Force -Recurse -ErrorAction Ignore"
powershell -Command "Remove-Item %LOG_FILE% -Force -Recurse -ErrorAction Ignore"

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
del "%FILES_FILE%"
