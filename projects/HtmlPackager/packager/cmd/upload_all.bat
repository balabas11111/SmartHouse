@ECHO OFF

echo ====================================================
echo    upload gz files http://%IP%/edit
echo ====================================================
 
SET URL_PREFFIX=/web/

SET RESULT_FILE=%PROJECT_TARGET%\upload.log
 	
for %%f in (%PROJECT_TARGET%\*.gz) do (
	echo upload %URL_PREFFIX%%%~nxf
	echo DELETE %URL_PREFFIX%%%~nxf-------------->> %RESULT_FILE%
	curl -s -i -X DELETE -H "Expect:" -F "filename=%URL_PREFFIX%%%~nxf" http://%IP%/edit >> %RESULT_FILE%
	echo UPLOAD %URL_PREFFIX%%%~nxf-------------->> %RESULT_FILE%
	curl -s -i -X POST -H "Expect:" -F "uploadfile=@%%f;filename=%URL_PREFFIX%%%~nxf;" -F submit=upload http://%IP%/edit >> %RESULT_FILE% 
)
echo ====================================================
echo        IP call result
echo ====================================================
type "%RESULT_FILE%"
echo: 
echo ====================================================

