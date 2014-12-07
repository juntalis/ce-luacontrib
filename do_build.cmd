@echo off
setlocal
if not defined INCLUDE goto wrong_script
goto startup

:wrong_script
echo Do not run this script directly. Instead, run build.cmd..
echo.
echo Or just comment out line 3 and do whatever you want.
goto :EOF

:startup
set BUILD_CFG=ndebug
if /i "%~1x"=="debugx" set BUILD_CFG=debug

set ROOTDIR=%~dp0
set OBJDIR=%ROOTDIR%obj
set BINDIR=%ROOTDIR%bin
set VENDORDIR=%ROOTDIR%vendor
set ROOTDIR=%ROOTDIR:~0,-1%
set INCDIRS="-I%ROOTDIR%" "-I%VENDORDIR%\CESDK" "-I%VENDORDIR%\MemoryModule"

if /i "%Platform%x"=="x86x" set PlatformBits=32
if /i "%Platform%x"=="x64x" set PlatformBits=64
if not defined PlatformBits set PlatformBits=32
if not defined Platform set Platform=x86
goto common_cfg

:compile_sources_process_arg
set OUTFILE_BASE=%OBJDIR%\%~n1
set OUTFILE_OBJ=%OUTFILE_BASE%.obj
set OUTFILE_CPP=%OUTFILE_BASE%.pp.c
set OUTFILE_DBG=%OUTFILE_BASE%.pdb
set CFLAGS_DBG=
if "%BUILD_CFG%x"=="debugx" set CFLAGS_DBG="-Fd%OUTFILE_DBG%"
call cl.exe %CFLAGS% %CFLAGS_DBG% "-Fo%OUTFILE_OBJ%" -c "%~1"
if defined DO_CPP ((cl.exe %CFLAGS% -EP "%~1"|"%BINDIR%\noblanks.exe")>"%OUTFILE_CPP%")
if errorlevel 1 exit /B %ERRORLEVEL%
goto :EOF

:compile_sources_loop
set COMPILE_ARG=%~1
if "%COMPILE_ARG%x"=="x" goto :EOF
call :compile_sources_process_arg "%COMPILE_ARG%"
if errorlevel 1 exit /B %ERRORLEVEL%
shift /1
goto compile_sources_loop

:compile_ffi
setlocal
set CFLAGS=%CFLAGS_BASE% %CFLAGS_FFI%
call :compile_sources_loop %SOURCES_FFI%
if errorlevel 1 (
	endlocal & exit /B %ERRORLEVEL%
)
endlocal
goto :EOF

:compile_plugin
setlocal
set CFLAGS=%CFLAGS_BASE%
call :compile_sources_loop %SOURCES_PLUGIN%
if errorlevel 1 (
	endlocal & exit /B %ERRORLEVEL%
)
endlocal
goto :EOF

:link_plugin
setlocal
set ERRCODE=0
set TMPMANIFEST=%TEMP%\%RANDOM%%RANDOM%.manifest
call link.exe %LDFLAGS% -DLL "-OUT:%BINDIR%\ce-luacontrib-%PlatformBits%.dll" "-MANIFESTFILE:%TMPMANIFEST%" "%OBJDIR%\*.obj" "%OBJDIR%\*.res"
if errorlevel 1 set ERRCODE=%ERRORLEVEL%
if exist "%TMPMANIFEST%" del /F /Q "%TMPMANIFEST%">nul 2>nul
if not "%ERRCODE%x"=="0x" exit /B %ERRCODE%
endlocal
goto :EOF

:common_cfg
set CFLAGS=-nologo -W3 -WX- -D_CRT_SECURE_NO_DEPRECATE -EHs-c- -Zc:wchar_t -Zc:forScope -D_WIN32
set LDFLAGS=-nologo -INCREMENTAL:NO -MACHINE:%Platform:x=X% -LARGEADDRESSAWARE -MANIFEST
if "x%PlatformBits%"=="x64" set CFLAGS=%CFLAGS% -D_WIN64
goto %BUILD_CFG%

:debug
set CFLAGS=%CFLAGS% -Zi -MDd -Od -DDEBUG -D_DEBUG -GS
set LDFLAGS=%LDFLAGS% -DEBUG "-PDB:%BINDIR%\ce-luacontrib-%PlatformBits%.pdb"
goto main

:ndebug
set CFLAGS=%CFLAGS% -Qfast_transcendentals -Ox -Og -Ob2 -Oi -GA -GL -GF -Gm- -GS- -Gy -GT -fp:precise -DNDEBUG=1 -D_NDEBUG=1
set LDFLAGS=%LDFLAGS% -LTCG -OPT:REF -OPT:ICF=16
if "x%PlatformBits%"=="x32" set CFLAGS=%CFLAGS% -Oy -arch:SSE2
if "x%PlatformBits%"=="x64" set CFLAGS=%CFLAGS% -favor:INTEL64 -arch:AVX
goto main

:main
if /i "%~1x"=="cppx" set DO_CPP=1
set FFI_DIR=%VENDORDIR%\ffi
set CFLAGS_BASE=%CFLAGS% %INCDIRS%
set CFLAGS_FFI="-I%ROOTDIR%\lua-fake" "-I%FFI_DIR%" "-I%FFI_DIR%\msvc"

set SOURCES_FFI="%FFI_DIR%\ffi.c" "%FFI_DIR%\call.c" "%FFI_DIR%\ctype.c" "%FFI_DIR%\parser.c"
set SOURCES_PLUGIN="%ROOTDIR%\ce-luacontrib.c" "%ROOTDIR%\luadll.c" "%ROOTDIR%\winutils.c" "%VENDORDIR%\MemoryModule\MemoryModule.c"

pushd "%ROOTDIR%"
if exist "%OBJDIR%" rmdir /S /Q "%OBJDIR%"
if not exist "%BINDIR%" mkdir "%BINDIR%"
mkdir "%OBJDIR%"

call rc.exe /nologo "/fo%OBJDIR%\ce-luacontrib.res" "%ROOTDIR%\packaged\ce-luacontrib.rc"
if errorlevel 1 goto call_fail

rem call :compile_ffi
rem if errorlevel 1 goto call_fail

call :compile_plugin
if errorlevel 1 goto call_fail

call :link_plugin
if errorlevel 1 goto call_fail

goto cleanup

:call_fail
set BUILD_ERRNO=%ERRORLEVEL%

:cleanup
popd
if defined BUILD_ERRNO (
	echo Build failed. (ERRORLEVEL = %BUILD_ERRNO%)
	endlocal & exit /B %BUILD_ERRNO%
)
endlocal
:cleanup_success
echo Build successful.