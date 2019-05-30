@echo off
cls

:: Get script directory
set SCRIPT_DIR=%~dp0
set SCRIPT_DIR=%SCRIPT_DIR:~0,-1%

platformio ci --lib="." --verbose --board=huzzah "%SCRIPT_DIR%\examples\ESPCrashMonitorExample\ESPCrashMonitorExample.ino"

exit %ERRORLEVEL%