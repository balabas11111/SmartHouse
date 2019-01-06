SET REAL_PATH=%~dp0

SET BASE_WEB_PATH=%REAL_PATH:~0,-1%\

@ECHO OFF
cls
echo ======================================================================
echo ----------------------------BUILD STARTED-----------------------------
echo ======================================================================
echo PROJECT_PATH=%PROJECT_PATH%
echo BASE_WEB_PATH=%BASE_WEB_PATH%

SET TEMP_FOLDER_NAME=target
SET TEMP_FOLDER_CREATE_PATH=%PROJECT_PATH%%TEMP_FOLDER_NAME%
SET TEMP_FOLDER=%PROJECT_PATH%%TEMP_FOLDER_NAME%\
SET LOG_FILE=%TEMP_FOLDER%build.log

SET PROJECT_HTML_FOLDER=%PROJECT_PATH%html\
SET PROJECT_JS_FOLDER=%PROJECT_PATH%js\

SET BASE_CSS_FOLDER=%BASE_WEB_PATH%css\
SET BASE_FONT_FOLDER=%BASE_WEB_PATH%font\
SET BASE_JS_FOLDER=%BASE_WEB_PATH%js\
SET BASE_HTML_FOLDER=%BASE_WEB_PATH%html\
SET BASE_HTML_COMPONENTS_FOLDER=%BASE_WEB_PATH%html_components\

SET PLACEHOLDERS_NAME=placeholders
SET PROJECT_PLACEHOLDERS_FOLDER=%PROJECT_PATH%%PLACEHOLDERS_NAME%
SET TEMP_HTML_PLACEHOLDERS_FOLDER=%TEMP_FOLDER%%PLACEHOLDERS_NAME%
SET BASE_HTML_PLACEHOLDERS_FOLDER=%BASE_HTML_COMPONENTS_FOLDER%%PLACEHOLDERS_NAME%\

@echo Build started > %LOG_FILE%
echo ----------------------------------------------------------------------
echo   'Recreate deployment folder'
echo ----------------------------------------------------------------------
@echo ON
powershell -Command "Remove-Item %TEMP_FOLDER_CREATE_PATH% -Force -Recurse -ErrorAction Ignore"
powershell -Command "New-Item -ItemType Directory -Force -Path %TEMP_FOLDER_CREATE_PATH%"
powershell -Command "New-Item -ItemType Directory -Force -Path %TEMP_HTML_PLACEHOLDERS_FOLDER%"
@echo OFF

SET /a itemCount=0

echo ======================================================================
echo   'Copy Files'
echo ======================================================================
echo .
echo ----------------------------------------------------------------------
echo   'Copy BASE htm to build folder'
echo ----------------------------------------------------------------------
for %%h in (%BASE_HTML_FOLDER%*.htm) do (
	echo copy %%~nxh -- dest=%TEMP_FOLDER_CREATE_PATH%
	powershell -Command "Copy-Item %BASE_HTML_FOLDER%%%~nxh -Destination %TEMP_FOLDER_CREATE_PATH%"
)

echo '
echo ----------------------------------------------------------------------
echo   'copy Project htm files to build folder'
echo ----------------------------------------------------------------------

for %%h in (%PROJECT_HTML_FOLDER%*.htm) do (
	echo copy %%~nxh -- dest=%TEMP_FOLDER_CREATE_PATH%
	powershell -Command "Copy-Item %PROJECT_HTML_FOLDER%%%~nxh -Destination %TEMP_FOLDER_CREATE_PATH%"
)

echo ======================================================================
echo    'Convert BASE html Components. Project placeholder %PROJECT_PLACEHOLDERS_FOLDER%\*'
echo ======================================================================

for /D %%d in (%PROJECT_PLACEHOLDERS_FOLDER%\*) do (
	
	echo %%~d\*.html
	
	for %%e in (%%~d\*.html) do (
		
		echo ------------------------------------------------------------------------
		echo        convert %%~nxe to %%~nxd.html
		echo ------------------------------------------------------------------------
		
		echo BASE_FILE %BASE_HTML_PLACEHOLDERS_FOLDER%%%~nxe
		echo TARGET_FILE %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html
		
		powershell -Command "Copy-Item %BASE_HTML_PLACEHOLDERS_FOLDER%%%~nxe -Destination %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html
		
		for %%g in (%%~d\*.txt) do (
			find /I "{%%~nxg}" %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html >> %LOG_FILE%  && (
					echo replace '{%%~nxg}' in %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html content=%%~d\%%~nxg
					powershell -Command "(gc %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html -Encoding UTF8) -replace '{%%~nxg}', (gc %%~d\%%~nxg -Encoding UTF8) | Out-File %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html"
				) || (@echo '{%%~nxg}' not found in %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nd.html)
		)
	)
)

echo ======================================================================
echo                 'Process CSS and Fonts'
echo ======================================================================
for %%h in (%TEMP_FOLDER%*.htm) do (
echo ----------------------------------------------------------------------
echo ----------------------------------------------------------------------
echo   'process file %%~nxh'
echo ======================================================================

echo ----------------------------------------------------------------------
echo   'Replace CSS from %BASE_CSS_FOLDER%'
echo ----------------------------------------------------------------------

	for %%g in (%BASE_CSS_FOLDER%*.css) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && ( 
			echo replace '{%%~nxg}' in %%~nxh content=%BASE_CSS_FOLDER%%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %BASE_CSS_FOLDER%%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)

echo ----------------------------------Process Fonts----------------------
echo    Replace FONTS from %BASE_FONT_FOLDER%
echo ---------------------------------------------------------------------

	for %%g in (%BASE_FONT_FOLDER%*.woff2) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && ( 
			echo replace '{%%~nxg}' in %%~nxh content=%BASE_FONT_FOLDER%%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %BASE_FONT_FOLDER%%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)

echo ======================================================================
echo                     'Process JS
echo ======================================================================
echo    'Replace BASE JS from %BASE_JS_FOLDER%'
echo ----------------------------------------------------------------------

	for %%g in (%BASE_JS_FOLDER%*.js) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && ( 
			echo replace '{%%~nxg}' in %%~nxh content=%BASE_JS_FOLDER%%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %BASE_JS_FOLDER%%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)
echo ----------------------------------------------------------------------
echo    'Replace PROJECT JS from %PROJECT_JS_FOLDER%'
echo ----------------------------------------------------------------------

	for %%g in (%PROJECT_JS_FOLDER%*.js) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && ( 
			echo replace '{%%~nxg}' in %%~nxh content=%PROJECT_JS_FOLDER%%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %PROJECT_JS_FOLDER%%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)

echo ======================================================================
echo                     'Process HTML Pages
echo ======================================================================
echo ----------------------------------------------------------------------
echo     'Replace BASE html pages %BASE_HTML_FOLDER%%%~nh'
echo ----------------------------------------------------------------------

	for %%g in (%BASE_HTML_FOLDER%%%~nh\*.html) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && (
			echo replace '{%%~nxg}' in %%~nxh content=%BASE_HTML_FOLDER%%%~nh%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %BASE_HTML_FOLDER%%%~nh\%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)

echo ----------------------------------------------------------------------
echo     'Replace PROJECT html pages %PROJECT_HTML_FOLDER%%%~nh
echo ----------------------------------------------------------------------


	for %%g in (%PROJECT_HTML_FOLDER%%%~nh\*.html) do (
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && (
			echo replace '{%%~nxg}' in %%~nxh content=%PROJECT_HTML_FOLDER%%%~nh%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %PROJECT_HTML_FOLDER%%%~nh\%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)

echo ======================================================================
echo    'Replace BASE html Components %BASE_HTML_COMPONENTS_FOLDER%'
echo ======================================================================

	for /D %%d in (%BASE_HTML_COMPONENTS_FOLDER%*) do (
		echo ----------------------------------------------------------------------	
		echo   -HtmlComponents folder '%BASE_HTML_COMPONENTS_FOLDER%%%~nxd\'
		
		for %%g in (%BASE_HTML_COMPONENTS_FOLDER%%%~nxd\*.html) do (
			find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && (
				echo replace '{%%~nxg}' in %%~nxh content=%BASE_HTML_COMPONENTS_FOLDER%%%~nxd\%%~nxg
				powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %BASE_HTML_COMPONENTS_FOLDER%%%~nxd\%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
			) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
		)
	)
	
echo ======================================================================
echo    'Replace PROJECT html Components from %TEMP_HTML_PLACEHOLDERS_FOLDER%'
echo ======================================================================

	for %%g in (%TEMP_HTML_PLACEHOLDERS_FOLDER%\*.html) do (
		echo ''replace '{%%~nxg}' in %%~nxh content=%TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nxg
		find /I "{%%~nxg}" %TEMP_FOLDER%%%~nxh >> %LOG_FILE%  && (
			echo replace '{%%~nxg}' in %%~nxh content=%TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nxg
			powershell -Command "(gc %TEMP_FOLDER%%%~nxh -Encoding UTF8) -replace '{%%~nxg}', (gc %TEMP_HTML_PLACEHOLDERS_FOLDER%\%%~nxg -Encoding UTF8) | Out-File %TEMP_FOLDER%%%~nxh"
		) || (@echo {%%~nxg} not found in %TEMP_FOLDER%%%~nxh >> %LOG_FILE%)
	)
echo ----------------------------------------------------------------------
echo    complete %%~nxh
echo ----------------------------------------------------------------------
)

echo ======================================================================
echo             RESULT folder = %TEMP_FOLDER%
echo ======================================================================
echo         'Htm files
echo ----------------------------------------------------------------------	
for %%h in (%TEMP_FOLDER%*.htm) do (
	echo %%~nxh
	%BASE_WEB_PATH%!gzip\gzip.exe %TEMP_FOLDER%%%~nxh -k
	
)
echo ----------------------------------------------------------------------	
echo         'Gzip files
echo ----------------------------------------------------------------------	
for %%g in (%TEMP_FOLDER%*.gz) do (
	echo %%~nxg
)
echo ======================================================================
