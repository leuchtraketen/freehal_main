@echo off
:start
del /Q saved\de\*.pro
del /Q saved\en\*.pro
hal2009-online-demo.exe ram
goto :start
