@echo off
for /f "delims=" %%i in ('dir /b "%~dp0*.pro"') do %~dp0protoc -I="%~dp0" --cpp_out="%~dp0" "%~dp0%%i"
pause