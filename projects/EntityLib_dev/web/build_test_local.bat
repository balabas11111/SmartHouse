SET IP=192.168.0.120
SET DELETE_HTML=False
SET UPLOAD_GZ=False

SET PROJECT_PATH=%cd%

call %PROJECT_PATH%\..\..\HtmlPackager\packager\target\build.bat

copy %PROJECT_PATH%\testData\EntityLib.json %PROJECT_PATH%\target\EntityLib.json
powershell -Command "(gc %PROJECT_PATH%\target\index.htm -encoding UTF8) -replace '/data', 'file://D:/projects/!!!SmartHouse/projects/EntityLib_dev/web/target/EntityLib.json' | Out-File -encoding UTF8 %PROJECT_PATH%\target\index.htm"
powershell -Command "(gc %PROJECT_PATH%\target\index.htm -encoding UTF8) -replace 'DATA_MODEL_SUBMIT_METHOD,', 'DATA_MODEL_REQUEST_METHOD,' | Out-File -encoding UTF8 %PROJECT_PATH%\target\index.htm"