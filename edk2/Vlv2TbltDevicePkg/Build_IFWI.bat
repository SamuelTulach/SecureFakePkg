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

SetLocal EnableDelayedExpansion EnableExtensions

@REM Go to work space directory.
cd ..
cd ..

:: Assign initial values
set exitCode=0
set "Build_Flags= "
set "Stitch_Flags= "
set Arch=X64
set WORKSPACE=%CD%
set CORE_PATH=%WORKSPACE%\edk2
set PLATFORM_PATH=%WORKSPACE%\edk2
set PLATFORM_PACKAGE=Vlv2TbltDevicePkg

:: Parse Optional arguments
:OptLoop
if /i "%~1"=="/?" goto Usage

if /i "%~1"=="/q" (
    set Build_Flags=%Build_Flags% /q
    shift
    goto OptLoop
)
if /i "%~1"=="/l" (
    set Build_Flags=%Build_Flags% /l
    shift
    goto OptLoop
)
if /i "%~1"=="/y" (
    set Build_Flags=%Build_Flags% /y
    shift
    goto OptLoop
)
if /i "%~1"=="/m" (
    set Build_Flags=%Build_Flags% /m
    shift
    goto OptLoop
)
if /i "%~1" == "/c" (
    set Build_Flags=%Build_Flags% /c
    shift
    goto OptLoop
)
if /i "%~1" == "/ECP" (
    set Build_Flags=%Build_Flags% /ecp
    shift
    goto OptLoop
)

if /i "%~1"=="/s" (
    set Build_Flags=%Build_Flags% /s
    shift
    goto OptLoop
)

if /i "%~1"=="/x64" (
    set Arch=X64
    set Build_Flags=%Build_Flags% /x64
    shift
    goto OptLoop
)

if /i "%~1"=="/IA32" (
    set Arch=IA32
    set Build_Flags=%Build_Flags% /IA32
    shift
    goto OptLoop
)

if /i "%~1"=="/nG" (
    set Stitch_Flags=%Stitch_Flags% /nG
    shift
    goto OptLoop
)
if /i "%~1"=="/nM" (
    set Stitch_Flags=%Stitch_Flags% /nM
    shift
    goto OptLoop
)
if /i "%~1"=="/nB" (
    set Stitch_Flags=%Stitch_Flags% /nB
    shift
    goto OptLoop
)
if /i "%~1"=="/yL" (
    set Stitch_Flags=%Stitch_Flags% /yL
    shift
    goto OptLoop
)


:: Require 2 input parameters
if "%~2"=="" goto Usage

:: Assign required arguments
set Platform_Type=%~1
set Build_Target=%~2

if "%~3"=="" (
    set "IFWI_Suffix= "
) else set "IFWI_Suffix=/S %~3"

:: Build BIOS
echo ======================================================================
echo Build_IFWI:  Calling BIOS build Script...

call %PLATFORM_PATH%\%PLATFORM_PACKAGE%\bld_vlv.bat %Build_Flags%  %Platform_Type% %Build_Target%

if %ERRORLEVEL% NEQ 0 (
    echo echo  -- Error Building BIOS  & echo.
    set exitCode=1
    goto exit
)
echo.
echo Finished Building BIOS.
@REM Set BIOS_ID environment variable here.
call %CORE_PATH%\Conf\BiosId.bat
echo BIOS_ID=%BIOS_ID%

:: Set the Board_Id, Build_Type, Version_Major, and Version_Minor environment variables
find /v "#" %CORE_PATH%\Conf\BiosId.env > ver_strings
for /f "tokens=1,3" %%i in (ver_strings) do set %%i=%%j
del /f/q ver_strings >nul
set BIOS_Name=%BOARD_ID%_%Arch%_%BUILD_TYPE%_%VERSION_MAJOR%_%VERSION_MINOR%.ROM

:: Start Integration process
echo ======================================================================
echo Build_IFWI:  Calling IFWI Stitching Script...
pushd %CORE_PATH%\%PLATFORM_PACKAGE%\Stitch

  :: IFWIStitch.bat [/nG] [/nM] [/nB] [/B BIOS.rom] [/C StitchConfig] [/S IFWISuffix]
  call IFWIStitch.bat %Stitch_Flags% /B ..\..\%BIOS_Name% %IFWI_Suffix%
   
 @echo off
popd
if %ERRORLEVEL% NEQ 0 (
    echo echo  -- Error Stitching %BIOS_Nam% & echo.
    set exitCode=1
)
echo.
echo Build_IFWI is finished.
echo The final IFWI file is located in %ROOT_DIR%\Vlv2TbltDevicePkg\Stitch\
echo ======================================================================
goto Exit

:Usage
echo Script to build BIOS firmware and stitch the entire IFWI.
echo.
echo Usage: Build_IFWI.bat [options]  PlatformType  BuildTarget  [IFWI Suffix]
echo.
echo        /c     CleanAll before building
echo        /x64   Set Arch to X64  (default: X64)
echo        /IA32  Set Arch to IA32 (default: X64)
echo        /yL    Enable SPI lock
echo. 
echo        Platform Types:   MNW2
echo        Build Targets:    Release, Debug
echo        IFWI Suffix:      Suffix to append to end of IFWI filename (default: MM_DD_YYYY)
echo.
echo        See  Stitch/Stitch_Config.txt  for additional stitching settings.
echo.
echo        If capsule update is needed, please update CAPSULE_ENABLE = TRUE in Config.dsc.
echo        If recovery is needed, please update RECOVERY_ENABLE = TRUE in Config.dsc.
echo        If either of above is TRUE, please set OPENSSL_PATH in windows evironment
echo        and put openssl.exe there, to generate final capsule image.
echo.
set exitCode=1

:Exit
@REM  CD to platform package.
cd %PLATFORM_PATH%
exit /b %exitCode%

EndLocal
