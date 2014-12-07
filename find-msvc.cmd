@echo off
rem Usage: find-msvc.cmd MinimumVersion MaximumVersion
rem 
rem Given a range of versions to search for, this script attempts to locate a valid Visual Studio installation based
rem upon the current set of environment variables. If a valid installation is found, this script sets the environment
rem variable "VCROOT" to  Drive:\Path\To\VS\VC. On success, we should be able to call %VCROOT%\vcvarsall.bat.
rem
rem Versions use the VSXX0COMNTOOLS environment variable:
rem 
rem     VS 2013 => 12
rem     VS 2012 => 11
rem     VS 2010 => 10
rem     VS 2008 => 9
rem     VS 2005 => 8
rem 
rem Example
rem     call find-msvc.cmd" 8 10
rem     if errorlevel 1 exit /B 1
rem     call "%VCROOT%\vcvarsall.bat"
rem
setlocal

if not defined CMD_FORK_LEVEL (
	set /A CMD_FORK_LEVEL=1
) else (
	set /A CMD_FORK_LEVEL=%CMD_FORK_LEVEL% + 1
)
if %CMD_FORK_LEVEL% GTR 1 goto forked
set MIN_VERSION=%~1
set MAX_VERSION=%~2
if "%~1x"=="x" set MIN_VERSION=8
if "%~2x"=="x" set MAX_VERSION=12
goto main

:forked
if not "%VSTARGETCOMN%x"=="x" goto cleanup
call set "TESTVERS=%%VS%~10COMNTOOLS%%"
if "%TESTVERS%x"=="x" goto cleanup
set RESULT=VS%~10COMNTOOLS
endlocal & set VSTARGETCOMN=%RESULT%
goto :EOF

:cleanup
endlocal
goto :EOF

:lowercase
rem Subroutine to convert a variable VALUE to all lower case.
rem The argument for this subroutine is the variable NAME.
for %%i in ("A=a" "B=b" "C=c" "D=d" "E=e" "F=f" "G=g" "H=h" "I=i" "J=j" "K=k" "L=l" "M=m" "N=n" "O=o" "P=p" "Q=q" "R=r" "S=s" "T=t" "U=u" "V=v" "W=w" "X=x" "Y=y" "Z=z") do @call set "%1=%%%1:%%~i%%"
goto :EOF

:main
set VSTARGETCOMN=
for /L %%Z in (%MAX_VERSION%,-1,%MIN_VERSION%) do @call "%~f0" %%~Z
call set VCROOT=%%%VSTARGETCOMN%%%
if "%VCROOT%x"=="x" goto notfound
call :lowercase VCROOT
if "%VCROOT:~-1%"=="\" set VCROOT=%VCROOT:~0,-1%
endlocal & set VCROOT=%VCROOT:\common7\tools=\VC%
goto :EOF

:notfound
echo Could not locate a valid MSVC version.
endlocal
exit /B 1
