@REM echo off
set targetPath="%1"

set targetPath="D:\github\game_v11\core\debug\lua51.dll"

set CURDIR=%cd%

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

set outname=%targetPath%

@REM 依赖
%MYDIR%/VC/bin/dumpbin /dependents %targetPath%

@REM 打印导出的函数
%MYDIR%/VC/bin/dumpbin /exports %targetPath%

pause