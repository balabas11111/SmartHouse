echo                begin file upload 
echo ====================================================
echo                check IP=%IP% 
echo ====================================================

@ECHO OFF

set RESULT_FILE=%TEMP_FOLDER%ipres.txt
set FILES_FILE=%TEMP_FOLDER%files.txt

rem set URL_PREFFIX=%TEMP_FOLDER_NAME%/
set URL_PREFFIX=/web/

curl -Is %IP% > %RESULT_FILE% 

find /I "HTTP" %RESULT_FILE% >> %LOG_FILE%  && (
		echo IP returned response
		
	) || (
		echo ERROR ip is not available!!!
		
		GOTO:EXIT_LABEL
	)

echo ====================================================
echo    upload built files http://%IP%/edit
echo ====================================================
echo -----------------Upload files----------------------- >> %RESULT_FILE% 	
echo: >> %RESULT_FILE%
for %%f in (%TEMP_FOLDER_NAME%\*.gz) do (
	echo %URL_PREFFIX%%%~nxf
	curl -s -i -X DELETE -H "Expect:" -F "filename=%URL_PREFFIX%%%~nxf" http://%IP%/edit >> %RESULT_FILE%
	curl -s -i -X POST -H "Expect:" -F "uploadfile=@%%f;filename=%URL_PREFFIX%%%~nxf;" -F submit=upload http://%IP%/edit  >> %RESULT_FILE%
)

powershell -Command "Remove-Item %LOG_FILE% -Force -Recurse -ErrorAction Ignore"
powershell -Command "Remove-Item %RESULT_FILE% -Force -Recurse -ErrorAction Ignore"

GOTO:EXIT_LABEL

REM powershell -Command "Remove-Item %RESULT_FILE% -Force -Recurse -ErrorAction Ignore"	
	
echo ====================================================
echo        IP call result
echo ====================================================
type "%RESULT_FILE%"
echo: 
echo ====================================================

:EXIT_LABEL
echo ====================================================

