SET IP=192.168.0.107
SET FOLDER=css
SET PREFFIX=web
SET DELIM=/

SET TEMP_FOLDER=tempDeploy
SET BASE_WEB_PATH=..\..\web_base\

SET CSS_FOLDER=css\
SET HTML_FOLDER=html\
SET JS_FOLDER=js\

@ECHO OFF

echo '---------------Recreate deployment folder---------------------'
powershell -Command "Remove-Item %TEMP_FOLDER% -Force -Recurse -ErrorAction Ignore"
powershell -Command "New-Item -ItemType Directory -Force -Path %TEMP_FOLDER%"

echo '---------------copy base css-------------'
powershell -Command "Copy-Item %BASE_WEB_PATH%%CSS_FOLDER%w3.css -Destination %TEMP_FOLDER%"
echo '---------------REM copy base js-------------'
powershell -Command "Copy-Item %BASE_WEB_PATH%%JS_FOLDER%DeviceLib.js -Destination %TEMP_FOLDER%"
powershell -Command "Copy-Item %BASE_WEB_PATH%%JS_FOLDER%SettingsBase.js -Destination %TEMP_FOLDER%"
echo '---------------REM copy base htmls---------------------'
powershell -Command "Copy-Item %BASE_WEB_PATH%index.htm -Destination %TEMP_FOLDER%"
powershell -Command "Copy-Item %BASE_WEB_PATH%settings.htm -Destination %TEMP_FOLDER%"
powershell -Command "Copy-Item %BASE_WEB_PATH%edit.htm.gz -Destination %TEMP_FOLDER%"

echo '---------------REM copy own JS---------------------'
powershell -Command "Copy-Item %JS_FOLDER%index.js -Destination %TEMP_FOLDER%"
powershell -Command "Copy-Item %JS_FOLDER%settings.js -Destination %TEMP_FOLDER%"
echo '---------------REM copy own html---------------------'
powershell -Command "Copy-Item %HTML_FOLDER%index.html -Destination %TEMP_FOLDER%"
powershell -Command "Copy-Item %HTML_FOLDER%settings.html -Destination %TEMP_FOLDER%"

echo '---------------Replace index.htm---------------------'
SET CURRENT_FILE=index.htm

SET REPL_FILE=w3.css
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<style> ' + \" `r`n\"  + (gc %TEMP_FOLDER%/%REPL_FILE%) + \" `r`n\" + '</style>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=DeviceLib.js
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<script id=\"%REPL_FILE%\" language=\"javascript\">'+\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\" + '</script>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=index.js
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<script id=\"%REPL_FILE%\" language=\"javascript\">'+\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\" + '</script>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=index.html
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', (\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\") | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
echo '---------------pack index.htm
%BASE_WEB_PATH%!gzip\gzip.exe %TEMP_FOLDER%\%CURRENT_FILE% -k

echo ------------------------------------------
echo '---------------Replace settings.htm---------------------'
SET CURRENT_FILE=settings.htm

SET REPL_FILE=w3.css
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<style> ' + \" `r`n\"  + (gc %TEMP_FOLDER%/%REPL_FILE%) + \" `r`n\" + '</style>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=DeviceLib.js
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<script id=\"%REPL_FILE%\" language=\"javascript\">'+\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\" + '</script>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=SettingsBase.js
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<script id=\"%REPL_FILE%\" language=\"javascript\">'+\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\" + '</script>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=settings.js
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', ('<script id=\"%REPL_FILE%\" language=\"javascript\">'+\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\" + '</script>') | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"
SET REPL_FILE=settings.html
powershell -Command "(gc %TEMP_FOLDER%\%CURRENT_FILE% -Encoding UTF8) -replace '%REPL_FILE%_replacement', (\" `r`n\" + (gc %TEMP_FOLDER%/%REPL_FILE% -Encoding UTF8) + \"`r`n\") | Out-File %TEMP_FOLDER%\%CURRENT_FILE%"

echo '---------------Replace settings.htm---------------------'
%BASE_WEB_PATH%!gzip\gzip.exe %TEMP_FOLDER%\%CURRENT_FILE% -k

echo '---------------------------------------------------------------'
