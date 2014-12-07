@echo off
setlocal

rem Find MSVC directory first.
call "%~dp0find-msvc.cmd" 8 10
if errorlevel 1 exit /B 1

setlocal
set Platform=x86
call "%VCROOT%\vcvarsall.bat" %Platform%>nul 2>nul
call "%~dp0do_build.cmd" %*
if errorlevel 1 goto err
endlocal

setlocal
set Platform=x64
call "%VCROOT%\vcvarsall.bat" %Platform%>nul 2>nul
call "%~dp0do_build.cmd" %*
if errorlevel 1 goto err
endlocal

endlocal
goto :EOF

:err
echo ERROR: One of the build processes failed. (ERRORLEVEL = %ERRORLEVEL%)
exit /B %ERRORLEVEL%
