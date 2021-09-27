@REM @file
@REM   Windows batch file to build BIOS ROM
@REM
@REM Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
@REM This program and the accompanying materials
@REM are licensed and made available under the terms and conditions of the BSD License
@REM which accompanies this distribution.  The full text of the license may be found at
@REM http://opensource.org/licenses/bsd-license.php
@REM
@REM THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
@REM WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
@REM

@echo off
setlocal EnableDelayedExpansion EnableExtensions
echo.
echo %date%  %time%
echo.


::**********************************************************************
:: Initial Setup
::**********************************************************************
set WORKSPACE=%CD%
if %WORKSPACE:~-1%==\ set WORKSPACE=%WORKSPACE:~0,-1%
set /a build_threads=1
set "Build_Flags= "
set exitCode=0
set Arch=X64
set Source=0
set CORE_PATH=%WORKSPACE%\edk2
set PLATFORM_PATH=%WORKSPACE%\edk2
set PACKAGES_PATH=%WORKSPACE%\silicon;%WORKSPACE%\edk2
cd ./edk2

:: Clean up previous build files.
if exist %CORE_PATH%\edk2.log del %CORE_PATH%\edk2.log
if exist %CORE_PATH%\unitool.log del %CORE_PATH%\unitool.log
if exist %CORE_PATH%\Conf\target.txt del %CORE_PATH%\Conf\target.txt
if exist %CORE_PATH%\Conf\tools_def.txt del %CORE_PATH%\Conf\tools_def.txt
if exist %CORE_PATH%\Conf\build_rule.txt del %CORE_PATH%\Conf\build_rule.txt
if exist %CORE_PATH%\Conf\FrameworkDatabase.db del %CORE_PATH%\Conf\FrameworkDatabase.db
if exist conf\.cache rmdir /q/s conf\.cache

:: Setup EDK environment. Edksetup puts new copies of target.txt, tools_def.txt, build_rule.txt in WorkSpace\Conf
:: Also run edksetup as soon as possible to avoid it from changing environment variables we're overriding
call %CORE_PATH%\edksetup.bat Rebuild
@echo off

:: Define platform specific environment variables.
set PLATFORM_NAME=Vlv2TbltDevicePkg
set PLATFORM_PACKAGE=%PLATFORM_PATH%\Vlv2TbltDevicePkg
set config_file=%PLATFORM_PACKAGE%\PlatformPkgConfig.dsc
set auto_config_inc=%PLATFORM_PACKAGE%\AutoPlatformCFG.txt



::create new AutoPlatformCFG.txt file
copy /y nul %auto_config_inc% >nul

::**********************************************************************
:: Parse command line arguments
::**********************************************************************

:: Optional arguments
:OptLoop
if /i "%~1"=="/?" goto Usage

if /i "%~1"=="/l" (
    set Build_Flags=%Build_Flags% -j EDK2.log
    shift
    goto OptLoop
)
if /i "%~1"=="/y" (
    set Build_Flags=%Build_Flags% -y %PLATFORM_PACKAGE%\EDK2_%PLATFORM_PACKAGE%.report
    shift
    goto OptLoop
)
if /i "%~1"=="/m" (
    if defined NUMBER_OF_PROCESSORS (
        set /a build_threads=%NUMBER_OF_PROCESSORS%+1
    )
    shift
    goto OptLoop
)
if /i "%~1" == "/c" (
    echo Removing previous build files ...
    if exist build (
        del /f/s/q build > nul
        rmdir /s/q build
    )
    if exist conf\.cache (
        del /f/s/q conf\.cache > nul
        rmdir /s/q conf\.cache
    )
    echo.
    shift
    goto OptLoop
)

if /i "%~1"=="/x64" (
    set Arch=X64
    shift
    goto OptLoop
)
if /i "%~1"=="/IA32" (
    set Arch=IA32
    shift
    goto OptLoop
)

:: Required argument(s)
if "%~1"=="" goto Usage

::Remove the values for Platform_Type and Build_Target from BiosIdX.env and stage in Conf\
if "%Arch%"=="IA32" (
    findstr /b /v "BOARD_ID  BUILD_TYPE" %PLATFORM_PACKAGE%\BiosIdR.env > Conf\BiosId.env
    echo DEFINE X64_CONFIG = FALSE  >> %auto_config_inc%
) else if "%Arch%"=="X64" (
    findstr /b /v "BOARD_ID  BUILD_TYPE" %PLATFORM_PACKAGE%\BiosIdx64R.env > Conf\BiosId.env
    echo DEFINE X64_CONFIG = TRUE  >> %auto_config_inc%
)

:: -- Build flags settings for each Platform --
echo Setting  %1  platform configuration and BIOS ID...
if /i "%~1" == "MNW2" (
    echo BOARD_ID = MNW2MAX >> Conf\BiosId.env
    echo DEFINE ENBDT_PF_BUILD = TRUE   >> %auto_config_inc%
    
) else (
    echo Error - Unsupported PlatformType: %1
    goto Usage
)
set Platform_Type=%~1

if /i "%~2" == "RELEASE" (
    set target=RELEASE
    echo BUILD_TYPE = R >> Conf\BiosId.env
) else (
    set target=DEBUG
    echo BUILD_TYPE = D >> Conf\BiosId.env
)

::**********************************************************************
:: Additional EDK Build Setup/Configuration
::**********************************************************************
echo.
echo Setting the Build environment for VS2015/VS2013/VS2012/VS2010/VS2008...
if defined VS140COMNTOOLS (
  if not defined VSINSTALLDIR call "%VS140COMNTOOLS%\vsvars32.bat"
  if /I "%VS140COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 14.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2015
  ) else (
    set TOOL_CHAIN_TAG=VS2015x86
  ) 
) else if defined VS120COMNTOOLS (
  if not defined VSINSTALLDIR call "%VS120COMNTOOLS%\vsvars32.bat"
  if /I "%VS120COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2013
  ) else (
    set TOOL_CHAIN_TAG=VS2013x86
  )
) else if defined VS110COMNTOOLS (
  if not defined VSINSTALLDIR call "%VS110COMNTOOLS%\vsvars32.bat"
  if /I "%VS110COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 11.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2012
  ) else (
    set TOOL_CHAIN_TAG=VS2012x86
  )
) else if defined VS100COMNTOOLS (
  if not defined VSINSTALLDIR call "%VS100COMNTOOLS%\vsvars32.bat"
  if /I "%VS100COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 10.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2010
  ) else (
    set TOOL_CHAIN_TAG=VS2010x86
  )
) else if defined VS90COMNTOOLS (
  if not defined VSINSTALLDIR call "%VS90COMNTOOLS%\vsvars32.bat"
  if /I "%VS90COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\" (
     set TOOL_CHAIN_TAG=VS2008
  ) else (
     set TOOL_CHAIN_TAG=VS2008x86
  )
) else (
  echo  --ERROR: VS2015/VS2013/VS2012/VS2010/VS2008 not installed correctly. VS140COMNTOOLS/VS120COMNTOOLS/VS110COMNTOOLS/VS100COMNTOOLS/VS90COMNTOOLS not defined ^^!
  echo.
  goto :BldFail
)

echo Ensuring correct build directory is present for GenBiosId...
set BUILD_PATH=Build\%PLATFORM_NAME%\%TARGET%_%TOOL_CHAIN_TAG%

echo Modifing Conf files for this build...
:: Remove lines with these tags from target.txt
findstr /V "TARGET  TARGET_ARCH  TOOL_CHAIN_TAG  BUILD_RULE_CONF  ACTIVE_PLATFORM  MAX_CONCURRENT_THREAD_NUMBER" Conf\target.txt > Conf\target.txt.tmp

echo TARGET          = %TARGET%                                  >> Conf\target.txt.tmp
if "%Arch%"=="IA32" (
    echo TARGET_ARCH = IA32                                       >> Conf\target.txt.tmp
) else if "%Arch%"=="X64" (
    echo TARGET_ARCH = IA32 X64                                  >> Conf\target.txt.tmp
)
echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                                  >> Conf\target.txt.tmp
echo BUILD_RULE_CONF = Conf/build_rule.txt                               >> Conf\target.txt.tmp
if %Source% == 0 (
  echo ACTIVE_PLATFORM = %PLATFORM_PACKAGE%/PlatformPkg%Arch%.dsc        >> Conf\target.txt.tmp
) else (
  echo ACTIVE_PLATFORM = %PLATFORM_PACKAGE%/PlatformPkg%Arch%Source.dsc  >> Conf\target.txt.tmp
)
echo MAX_CONCURRENT_THREAD_NUMBER = %build_threads%                      >> Conf\target.txt.tmp

move /Y Conf\target.txt.tmp Conf\target.txt >nul

::**********************************************************************
:: Build BIOS
::**********************************************************************

echo Creating BiosId...
pushd %PLATFORM_PACKAGE%
if not exist %WORKSPACE%\%BUILD_PATH%\IA32  mkdir %WORKSPACE%\%BUILD_PATH%\IA32
  GenBiosId.exe -i %CORE_PATH%\Conf\BiosId.env -o %WORKSPACE%\%BUILD_PATH%\IA32\BiosId.bin -ob %CORE_PATH%\Conf\BiosId.bat
if "%Arch%"=="X64" (
   if not exist %WORKSPACE%\%BUILD_PATH%\X64  mkdir %WORKSPACE%\%BUILD_PATH%\X64
   GenBiosId.exe -i %CORE_PATH%\Conf\BiosId.env -o %WORKSPACE%\%BUILD_PATH%\X64\BiosId.bin -ob %CORE_PATH%\Conf\BiosId.bat
)
popd


if %ERRORLEVEL% NEQ 0 goto BldFail

echo.
echo Invoking EDK2 build...
call build %Build_Flags%

if %ERRORLEVEL% NEQ 0 goto BldFail

::**********************************************************************
:: Post Build processing and cleanup
::**********************************************************************

echo Running fce...

pushd %PLATFORM_PACKAGE%
:: Extract Hii data from build and store in HiiDefaultData.txt
fce read -i %WORKSPACE%\%BUILD_PATH%\FV\Vlv.fd > %WORKSPACE%\%BUILD_PATH%\FV\HiiDefaultData.txt

:: save changes to VlvXXX.fd
fce update -i %WORKSPACE%\%BUILD_PATH%\FV\Vlv.fd -s %WORKSPACE%\%BUILD_PATH%\FV\HiiDefaultData.txt -o %WORKSPACE%\%BUILD_PATH%\FV\Vlv%Arch%.fd
popd

if %ERRORLEVEL% NEQ 0 goto BldFail
::echo FD successfully updated with default Hii values.

:: Set the Board_Id, Build_Type, Version_Major, and Version_Minor environment variables
find /v "#" Conf\BiosId.env > ver_strings
for /f "tokens=1,3" %%i in (ver_strings) do set %%i=%%j
del /f/q ver_strings >nul

set BIOS_Name=%BOARD_ID%_%Arch%_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%.ROM
copy /y/b %WORKSPACE%\%BUILD_PATH%\FV\Vlv%Arch%.fd  %PLATFORM_PATH%\%BIOS_Name% >nul
copy /y/b %WORKSPACE%\%BUILD_PATH%\FV\Vlv%Arch%.fd  %WORKSPACE%\%BUILD_PATH%\FV\Vlv.ROM >nul

echo.
echo Build location:     %BUILD_PATH%
echo BIOS ROM Created:   %BIOS_Name%
echo.
echo -------------------- The EDKII BIOS build has successfully completed. --------------------
echo.

@REM build capsule here
@REMif "%openssl_path%" == "" (
@REM    echo -- Error:  OPENSSL_PATH not set.  Capule and Recovery images not generated.
@REM    set exitCode=1
@REM    goto Exit
@REM
@REM echo > %BUILD_PATH%\FV\SYSTEMFIRMWAREUPDATECARGO.Fv
@REM build -p %PLATFORM_PACKAGE%\PlatformCapsule.dsc

goto Exit

:Usage
echo.
echo ***************************************************************************
echo Build BIOS rom for VLV platforms.
echo.
echo Usage: bld_vlv.bat [options] PlatformType [Build Target]
echo.
echo    /c    CleanAll before building
echo    /l    Generate build log file
echo    /y    Generate build report file
echo    /m    Enable multi-processor build
echo    /IA32 Set Arch to IA32 (default: X64)
echo    /X64  Set Arch to X64 (default: X64)
echo.
echo        Platform Types:  MNW2
echo        Build Targets:   Debug, Release  (default: Debug)
echo.
echo Examples:
echo    bld_vlv.bat MNW2                 : X64 Debug build for MinnowMax
echo    bld_vlv.bat /IA32 MNW2 release   : IA32 Release build for MinnowMax
echo.
echo ***************************************************************************
set exitCode=1
goto Exit

:BldFail
set exitCode=1
echo  -- Error:  EDKII BIOS Build has failed!
echo See EDK2.log for more details

:Exit
echo %date%  %time%
exit /b %exitCode%

EndLocal
