SET IP=192.168.0.120

SET PROJECT_PATH=%cd%
for %%a in ("%PROJECT_PATH%.\..") do set "PROJECT_NAME=%%~nxa"

SET PROJECT_SOURCE=%PROJECT_PATH%\pages
SET PROJECT_TARGET=%PROJECT_PATH%\target
SET PROJECT_COMP=%PROJECT_PATH%\components
SET COMMON_COMP=%PROJECT_PATH%\..\..\web_base

SET UPLOADER_PATH=%PROJECT_PATH%\..\..\HtmlPackager\packager\target

java -jar %UPLOADER_PATH%\EspHtmlPackager.jar -name "%PROJECT_NAME%" -source "%PROJECT_SOURCE%" -target "%PROJECT_TARGET%" -comp1 "%COMMON_COMP%" -comp2 "%PROJECT_COMP%" -ip "%IP%" -upload "True" -deleteHtml "True"