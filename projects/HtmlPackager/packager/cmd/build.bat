@ECHO OFF
cls

echo %IP%
echo %DELETE_HTML%
echo %UPLOAD_GZ%

echo %PROJECT_PATH%

SET UPLOADER_PATH=%PROJECT_PATH%\..\..\HtmlPackager\packager\target

for %%a in ("%PROJECT_PATH%.\..") do set "PROJECT_NAME=%%~nxa"

SET PROJECT_SOURCE=%PROJECT_PATH%\pages
SET PROJECT_TARGET=%PROJECT_PATH%\target
SET PROJECT_COMP=%PROJECT_PATH%\components
SET COMMON_COMP=%PROJECT_PATH%\..\..\web_base

java -jar %UPLOADER_PATH%\EspHtmlPackager.jar -name "%PROJECT_NAME%" -source "%PROJECT_SOURCE%" -target "%PROJECT_TARGET%" -comp1 "%COMMON_COMP%" -comp2 "%PROJECT_COMP%" -ip "%IP%"

if "%UPLOAD_GZ%" == "True" ( 
	call %UPLOADER_PATH%\upload_all.bat
)else (
	echo No upload file specified
	echo ===============================================
) 
