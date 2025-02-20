@REM echo off

set CURDIR=%cd%
echo ################################# cl ftfont: %CURDIR% #################################

@REM echo %CURDIR%

cd..
set GITHUB_DIR=%cd%
set MYDIR=%cd%\MicrosoftVisualStudio8

set Lib=%MYDIR%\VC\lib\;%MYDIR%\VC\PlatformSDK\Lib;
set Include=%MYDIR%\VC\include;%MYDIR%\VC\PlatformSDK\Include;
set PATH=%PATH%%MYDIR%/VC/bin/;%MYDIR%\Common7\IDE

@rem 增加本地include路径
set INCLUDE=%INCLUDE%%CURDIR%\lib_freetype2.9.1\include\;%CURDIR%\include\
@rem 增加本地lib路径
set LIB=%LIB%%CURDIR%\lib_freetype2.9.1\;

cd %CURDIR%

cd ftfont

"%MYDIR%/VC/bin/cl.exe" -c *.c /Ox /nologo

@REM pause

"%MYDIR%/VC/bin/link.exe" -dll -out:ftfont.dll *.obj

@REM 打印导出的函数
%MYDIR%/VC/bin/dumpbin /exports ftfont.dll

copy ftfont.dll %GITHUB_DIR%\game_v11\core\debug\ftfont.dll 
copy ftfont.lib %GITHUB_DIR%\game_v11\core\debug\ftfont.lib 

::@echo off
::提取txt、doc、xls文件总大小
for /f "delims=" %%a in ('dir /b *.dll') do set /a "s+=%%~za/1024"
echo,%s% kb

del *.obj *.exp *.lib *.dll

copy %CURDIR%\lib_freetype2.9.1\freetype.dll %GITHUB_DIR%\game_v11\core\debug\freetype.dll

cd %CURDIR%

pause