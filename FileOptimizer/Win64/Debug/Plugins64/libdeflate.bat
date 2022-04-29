@echo off
gzip.exe -cd %1 | gzip.exe -12 -f > %2
exit /b %ERRORLEVEL%