@echo off
del /f /q %1.sql > nul
del /f /q %2 > nul
echo PRAGMA optimize(0xfffe); > %1.sql
sqlite3.exe %1 .dump >> %1.sql
if %ERRORLEVEL% NEQ 0 goto error
sqlite3.exe %2 < %1.sql
if %ERRORLEVEL% NEQ 0 goto error
del /f /q %1.sql > nul
exit /b 0

:error
del /f /q %2 > nul
del /f /q %1.sql > nul
exit /b 1