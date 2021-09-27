#/** @file
# ARM platform package.
#
# Copyright (c) 2009 - 2010, Apple Inc. All rights reserved.<BR>
# Copyright (c) 2011 - 2018, ARM Ltd. All rights reserved.<BR>
# Copyright (c) 2016 - 2017, Linaro Ltd. All rights reserved.<BR>
#
#    This program and the accompanying materials
#    are licensed and made available under the terms and conditions of the BSD License
#    which accompanies this distribution. The full text of the license may be found at
#    http://opensource.org/licenses/bsd-license.php
#
#    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#**/

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = ArmPlatformPkg
  PLATFORM_GUID                  = 9ce08891-ac9c-476d-ab04-0c04d3a97544
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001A
  OUTPUT_DIRECTORY               = Build/ArmPlatform
  SUPPORTED_ARCHITECTURES        = ARM|AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

[BuildOptions]
  RELEASE_*_*_CC_FLAGS  = -DMDEPKG_NDEBUG
  *_*_*_CC_FLAGS  = -DDISABLE_NEW_DEPRECATED_INTERFACES

[PcdsFixedAtBuild]
  gArmTokenSpaceGuid.PcdFdBaseAddress|0x0
  gArmTokenSpaceGuid.PcdFdSize|0x1000

[LibraryClasses.common]
  ArmGicArchLib|ArmPkg/Library/ArmGicArchSecLib/ArmGicArchSecLib.inf
  ArmGicLib|ArmPkg/Drivers/ArmGic/ArmGicLib.inf
  ArmLib|ArmPkg/Library/ArmLib/ArmBaseLib.inf
  ArmPlatformLib|ArmPlatformPkg/Library/ArmPlatformLibNull/ArmPlatformLibNull.inf
  ArmPlatformStackLib|ArmPlatformPkg/Library/ArmPlatformStackLib/ArmPlatformStackLib.inf
  ArmMmuLib|ArmPkg/Library/ArmMmuLib/ArmMmuBaseLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  CacheMaintenanceLib|ArmPkg/Library/ArmCacheMaintenanceLib/ArmCacheMaintenanceLib.inf
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  LcdHwLib|ArmPlatformPkg/Library/LcdHwNullLib/LcdHwNullLib.inf
  LcdPlatformLib|ArmPlatformPkg/Library/LcdPlatformNullLib/LcdPlatformNullLib.inf
  LzmaDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  MemoryInitPeiLib|ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.inf
  NorFlashPlatformLib|ArmPlatformPkg/Library/NorFlashPlatformNullLib/NorFlashPlatformNullLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PlatformPeiLib|ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf
  PrePiLib|EmbeddedPkg/Library/PrePiLib/PrePiLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

  NULL|ArmPkg/Library/CompilerIntrinsicsLib/CompilerIntrinsicsLib.inf
  NULL|MdePkg/Library/BaseStackCheckLib/BaseStackCheckLib.inf

[LibraryClasses.common.PEIM]
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf

[LibraryClasses.common.SEC]
  ExtractGuidedSectionLib|EmbeddedPkg/Library/PrePiExtractGuidedSectionLib/PrePiExtractGuidedSectionLib.inf
  HobLib|EmbeddedPkg/Library/PrePiHobLib/PrePiHobLib.inf
  MemoryAllocationLib|EmbeddedPkg/Library/PrePiMemoryAllocationLib/PrePiMemoryAllocationLib.inf
  PrePiHobListPointerLib|ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf

[Components.common]
  ArmPlatformPkg/Drivers/LcdGraphicsOutputDxe/LcdGraphicsOutputDxe.inf
  ArmPlatformPkg/Drivers/NorFlashDxe/NorFlashDxe.inf
  ArmPlatformPkg/Drivers/PL061GpioDxe/PL061GpioDxe.inf
  ArmPlatformPkg/Drivers/PL180MciDxe/PL180MciDxe.inf
  ArmPlatformPkg/Drivers/SP805WatchdogDxe/SP805WatchdogDxe.inf

  ArmPlatformPkg/Library/ArmPlatformLibNull/ArmPlatformLibNull.inf
  ArmPlatformPkg/Library/ArmPlatformStackLib/ArmPlatformStackLib.inf
  ArmPlatformPkg/Library/LcdHwNullLib/LcdHwNullLib.inf
  ArmPlatformPkg/Library/LcdPlatformNullLib/LcdPlatformNullLib.inf
  ArmPlatformPkg/Library/NorFlashPlatformNullLib/NorFlashPlatformNullLib.inf
  ArmPlatformPkg/Library/PL011SerialPortLib/PL011SerialPortLib.inf
  ArmPlatformPkg/Library/PL011UartClockLib/PL011UartClockLib.inf
  ArmPlatformPkg/Library/PL011UartLib/PL011UartLib.inf
  ArmPlatformPkg/Library/PL031RealTimeClockLib/PL031RealTimeClockLib.inf
  ArmPlatformPkg/Library/PrePiHobListPointerLib/PrePiHobListPointerLib.inf

  ArmPlatformPkg/MemoryInitPei/MemoryInitPeiLib.inf
  ArmPlatformPkg/MemoryInitPei/MemoryInitPeim.inf

  ArmPlatformPkg/PlatformPei/PlatformPeim.inf
  ArmPlatformPkg/PlatformPei/PlatformPeiLib.inf

  ArmPlatformPkg/PrePeiCore/PrePeiCoreMPCore.inf
  ArmPlatformPkg/PrePeiCore/PrePeiCoreUniCore.inf

  ArmPlatformPkg/PrePi/PeiMPCore.inf
  ArmPlatformPkg/PrePi/PeiUniCore.inf

  ArmPlatformPkg/Library/ArmMaliDp/ArmMaliDp.inf
