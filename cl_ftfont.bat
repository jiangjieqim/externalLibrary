@REM echo off
set targetPath="%1"

set CURDIR=%cd%
echo ################################# cl ftfont: %CURDIR% #################################

call warn.bat

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

"%MYDIR%/VC/bin/cl.exe" %WARN% -c *.c /Ox /nologo

@REM pause

set outname=ftfont


"%MYDIR%/VC/bin/link.exe" -dll -out:%outname%.dll *.obj

@REM 打印导出的函数
%MYDIR%/VC/bin/dumpbin /exports %outname%.dll

@REM 静态符号表
@REM %MYDIR%/VC/bin/dumpbin /SYMBOLS ftfont.lib

@REM 依赖
%MYDIR%/VC/bin/dumpbin /dependents %outname%.dll

::@echo off
::提取txt、doc、xls文件总大小
for /f "delims=" %%a in ('dir /b *.dll') do set /a "s+=%%~za/1024"
echo,%s% kb

if %targetPath%=="" (
    echo ...
) else (
    @REM %GITHUB_DIR%\game_v11\core\debug    

    copy %outname%.dll %targetPath%\%outname%.dll 
    copy %CURDIR%\lib_freetype2.9.1\freetype.dll %targetPath%\freetype.dll
)
del *.obj *.exp *.lib *.dll

cd %CURDIR%