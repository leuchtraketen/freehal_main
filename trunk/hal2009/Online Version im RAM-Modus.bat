@echo off
:start
del /Q saved\de\*.pro
del /Q saved\en\*.pro
hal2012-online-demo.exe ram
goto :start
