## @file
#  EFI/Framework Open Virtual Machine Firmware (OVMF) platform
#
#  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
#  (C) Copyright 2016 Hewlett Packard Enterprise Development LP<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution. The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = Ovmf
  PLATFORM_GUID                  = 5a9e7754-d81b-49ea-85ad-69eaa7b1539b
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/Ovmf3264
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = NOOPT|DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = OvmfPkg/OvmfPkgIa32X64.fdf

  #
  # Defines for default states.  These can be changed on the command line.
  # -D FLAG=VALUE
  #
  DEFINE SECURE_BOOT_ENABLE      = FALSE
  DEFINE NETWORK_IP6_ENABLE      = FALSE
  DEFINE HTTP_BOOT_ENABLE        = FALSE
  DEFINE SMM_REQUIRE             = FALSE
  DEFINE TLS_ENABLE              = FALSE
  DEFINE TPM2_ENABLE             = FALSE
  DEFINE TPM2_CONFIG_ENABLE      = FALSE

  #
  # Flash size selection. Setting FD_SIZE_IN_KB on the command line directly to
  # one of the supported values, in place of any of the convenience macros, is
  # permitted.
  #
!ifdef $(FD_SIZE_1MB)
  DEFINE FD_SIZE_IN_KB           = 1024
!else
!ifdef $(FD_SIZE_2MB)
  DEFINE FD_SIZE_IN_KB           = 2048
!else
!ifdef $(FD_SIZE_4MB)
  DEFINE FD_SIZE_IN_KB           = 4096
!else
  DEFINE FD_SIZE_IN_KB           = 4096
!endif
!endif
!endif

[BuildOptions]
  GCC:RELEASE_*_*_CC_FLAGS             = -DMDEPKG_NDEBUG
  INTEL:RELEASE_*_*_CC_FLAGS           = /D MDEPKG_NDEBUG
  MSFT:RELEASE_*_*_CC_FLAGS            = /D MDEPKG_NDEBUG
!if $(TOOL_CHAIN_TAG) != "XCODE5"
  GCC:*_*_*_CC_FLAGS                   = -mno-mmx -mno-sse
!endif
!ifdef $(SOURCE_DEBUG_ENABLE)
  MSFT:*_*_X64_GENFW_FLAGS  = --keepexceptiontable
  GCC:*_*_X64_GENFW_FLAGS   = --keepexceptiontable
  INTEL:*_*_X64_GENFW_FLAGS = --keepexceptiontable
!endif

  #
  # Disable deprecated APIs.
  #
  MSFT:*_*_*_CC_FLAGS = /D DISABLE_NEW_DEPRECATED_INTERFACES
  INTEL:*_*_*_CC_FLAGS = /D DISABLE_NEW_DEPRECATED_INTERFACES
  GCC:*_*_*_CC_FLAGS = -D DISABLE_NEW_DEPRECATED_INTERFACES

[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000
  XCODE:*_*_*_DLINK_FLAGS =

# Force PE/COFF sections to be aligned at 4KB boundaries to support page level
# protection of DXE_SMM_DRIVER/SMM_CORE modules
[BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE]
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000
  XCODE:*_*_*_DLINK_FLAGS =

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/BaseAcpiTimerLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  BootLogoLib|MdeModulePkg/Library/BootLogoLib/BootLogoLib.inf
  FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
  CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|MdePkg/Library/BasePciExpressLib/BasePciExpressLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PciCapLib|OvmfPkg/Library/BasePciCapLib/BasePciCapLib.inf
  PciCapPciSegmentLib|OvmfPkg/Library/BasePciCapPciSegmentLib/BasePciCapPciSegmentLib.inf
  PciCapPciIoLib|OvmfPkg/Library/UefiPciCapPciIoLib/UefiPciCapPciIoLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsicSev.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  SerialPortLib|PcAtChipsetPkg/Library/SerialIoLib/SerialIoLib.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLibDevicePathProtocol/UefiDevicePathLibDevicePathProtocol.inf
  NvVarsFileLib|OvmfPkg/Library/NvVarsFileLib/NvVarsFileLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  UefiCpuLib|UefiCpuPkg/Library/BaseUefiCpuLib/BaseUefiCpuLib.inf
  SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  SerializeVariablesLib|OvmfPkg/Library/SerializeVariablesLib/SerializeVariablesLib.inf
  QemuFwCfgLib|OvmfPkg/Library/QemuFwCfgLib/QemuFwCfgDxeLib.inf
  VirtioLib|OvmfPkg/Library/VirtioLib/VirtioLib.inf
  LoadLinuxLib|OvmfPkg/Library/LoadLinuxLib/LoadLinuxLib.inf
  MemEncryptSevLib|OvmfPkg/Library/BaseMemEncryptSevLib/BaseMemEncryptSevLib.inf
!if $(SMM_REQUIRE) == FALSE
  LockBoxLib|OvmfPkg/Library/LockBoxLib/LockBoxBaseLib.inf
!endif
  CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
  FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf

!ifdef $(SOURCE_DEBUG_ENABLE)
  PeCoffExtraActionLib|SourceLevelDebugPkg/Library/PeCoffExtraActionLibDebug/PeCoffExtraActionLibDebug.inf
  DebugCommunicationLib|SourceLevelDebugPkg/Library/DebugCommunicationLibSerialPort/DebugCommunicationLibSerialPort.inf
!else
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
!endif

  ResetSystemLib|OvmfPkg/Library/ResetSystemLib/ResetSystemLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
!if $(TLS_ENABLE) == TRUE
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
!else
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLibCrypto.inf
!endif

!if $(SECURE_BOOT_ENABLE) == TRUE
  PlatformSecureLib|OvmfPkg/Library/PlatformSecureLib/PlatformSecureLib.inf
  TpmMeasurementLib|SecurityPkg/Library/DxeTpmMeasurementLib/DxeTpmMeasurementLib.inf
  AuthVariableLib|SecurityPkg/Library/AuthVariableLib/AuthVariableLib.inf
!else
  TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
  AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
!endif
  VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf

  TcpIoLib|MdeModulePkg/Library/DxeTcpIoLib/DxeTcpIoLib.inf

!if $(HTTP_BOOT_ENABLE) == TRUE
  HttpLib|MdeModulePkg/Library/DxeHttpLib/DxeHttpLib.inf
!endif

!if $(TLS_ENABLE) == TRUE
  TlsLib|CryptoPkg/Library/TlsLib/TlsLib.inf
!endif

  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
  SmbusLib|MdePkg/Library/BaseSmbusLibNull/BaseSmbusLibNull.inf
  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  XenHypercallLib|OvmfPkg/Library/XenHypercallLib/XenHypercallLib.inf

!if $(TPM2_ENABLE) == TRUE
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  Tcg2PhysicalPresenceLib|OvmfPkg/Library/Tcg2PhysicalPresenceLibQemu/DxeTcg2PhysicalPresenceLib.inf
  Tcg2PpVendorLib|SecurityPkg/Library/Tcg2PpVendorLibNull/Tcg2PpVendorLibNull.inf
!else
  Tcg2PhysicalPresenceLib|OvmfPkg/Library/Tcg2PhysicalPresenceLibNull/DxeTcg2PhysicalPresenceLib.inf
!endif

[LibraryClasses.common]
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf

[LibraryClasses.common.SEC]
  TimerLib|OvmfPkg/Library/AcpiTimerLib/BaseRomAcpiTimerLib.inf
  QemuFwCfgLib|OvmfPkg/Library/QemuFwCfgLib/QemuFwCfgSecLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformRomDebugLibIoPort.inf
!endif
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/BaseExtractGuidedSectionLib/BaseExtractGuidedSectionLib.inf
!ifdef $(SOURCE_DEBUG_ENABLE)
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SecPeiDebugAgentLib.inf
!endif
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf

[LibraryClasses.common.PEI_CORE]
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf

[LibraryClasses.common.PEIM]
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  ResourcePublicationLib|MdePkg/Library/PeiResourcePublicationLib/PeiResourcePublicationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
!ifdef $(SOURCE_DEBUG_ENABLE)
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SecPeiDebugAgentLib.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/PeiCpuExceptionHandlerLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  QemuFwCfgS3Lib|OvmfPkg/Library/QemuFwCfgS3Lib/PeiQemuFwCfgS3LibFwCfg.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  QemuFwCfgLib|OvmfPkg/Library/QemuFwCfgLib/QemuFwCfgPeiLib.inf

!if $(TPM2_ENABLE) == TRUE
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2DeviceLibDTpm.inf
!endif

[LibraryClasses.common.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib|MdeModulePkg/Library/DxeCoreMemoryAllocationLib/DxeCoreMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf
!ifdef $(SOURCE_DEBUG_ENABLE)
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/RuntimeDxeReportStatusCodeLib/RuntimeDxeReportStatusCodeLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/RuntimeCryptLib.inf
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf
  QemuFwCfgS3Lib|OvmfPkg/Library/QemuFwCfgS3Lib/DxeQemuFwCfgS3LibFwCfg.inf

[LibraryClasses.common.UEFI_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf

[LibraryClasses.common.DXE_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  NetLib|MdeModulePkg/Library/DxeNetLib/DxeNetLib.inf
  IpIoLib|MdeModulePkg/Library/DxeIpIoLib/DxeIpIoLib.inf
  UdpIoLib|MdeModulePkg/Library/DxeUdpIoLib/DxeUdpIoLib.inf
  DpcLib|MdeModulePkg/Library/DxeDpcLib/DxeDpcLib.inf
  PlatformBootManagerLib|OvmfPkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf
  PlatformBmPrintScLib|OvmfPkg/Library/PlatformBmPrintScLib/PlatformBmPrintScLib.inf
  QemuBootOrderLib|OvmfPkg/Library/QemuBootOrderLib/QemuBootOrderLib.inf
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/DxeCpuExceptionHandlerLib.inf
!if $(SMM_REQUIRE) == TRUE
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
!else
  LockBoxLib|OvmfPkg/Library/LockBoxLib/LockBoxDxeLib.inf
!endif
!ifdef $(SOURCE_DEBUG_ENABLE)
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/DxeDebugAgentLib.inf
!endif
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/DxeMpInitLib.inf
  QemuFwCfgS3Lib|OvmfPkg/Library/QemuFwCfgS3Lib/DxeQemuFwCfgS3LibFwCfg.inf
!if $(TPM2_ENABLE) == TRUE
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf
!endif

[LibraryClasses.common.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SmmCpuExceptionHandlerLib.inf
!ifdef $(SOURCE_DEBUG_ENABLE)
  DebugAgentLib|SourceLevelDebugPkg/Library/DebugAgent/SmmDebugAgentLib.inf
!endif
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf

[LibraryClasses.common.SMM_CORE]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  TimerLib|OvmfPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  SmmCorePlatformHookLib|MdeModulePkg/Library/SmmCorePlatformHookLibNull/SmmCorePlatformHookLibNull.inf
  MemoryAllocationLib|MdeModulePkg/Library/PiSmmCoreMemoryAllocationLib/PiSmmCoreMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
  SmmServicesTableLib|MdeModulePkg/Library/PiSmmCoreSmmServicesTableLib/PiSmmCoreSmmServicesTableLib.inf
!ifdef $(DEBUG_ON_SERIAL_PORT)
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!else
  DebugLib|OvmfPkg/Library/PlatformDebugLibIoPort/PlatformDebugLibIoPort.inf
!endif
  PciLib|OvmfPkg/Library/DxePciLibI440FxQ35/DxePciLibI440FxQ35.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform.
#
################################################################################
[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseMemory|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSupportUefiDecompress|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutUgaSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
!if $(SMM_REQUIRE) == TRUE
  gUefiOvmfPkgTokenSpaceGuid.PcdSmmSmramRequire|TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|FALSE
!endif

[PcdsFixedAtBuild]
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeMemorySize|1
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
  gEfiMdePkgTokenSpaceGuid.PcdMaximumGuidedExtractHandler|0x10
!if ($(FD_SIZE_IN_KB) == 1024) || ($(FD_SIZE_IN_KB) == 2048)
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x2000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxAuthVariableSize|0x2800
!if $(TLS_ENABLE) == FALSE
  # match PcdFlashNvStorageVariableSize purely for convenience
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableStoreSize|0xe000
!endif
!endif
!if $(FD_SIZE_IN_KB) == 4096
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x8400
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxAuthVariableSize|0x8400
!if $(TLS_ENABLE) == FALSE
  # match PcdFlashNvStorageVariableSize purely for convenience
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableStoreSize|0x40000
!endif
!endif
!if $(TLS_ENABLE) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVariableStoreSize|0x80000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVolatileVariableSize|0x40000
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0

  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07

  # DEBUG_INIT      0x00000001  // Initialization
  # DEBUG_WARN      0x00000002  // Warnings
  # DEBUG_LOAD      0x00000004  // Load events
  # DEBUG_FS        0x00000008  // EFI File system
  # DEBUG_POOL      0x00000010  // Alloc & Free (pool)
  # DEBUG_PAGE      0x00000020  // Alloc & Free (page)
  # DEBUG_INFO      0x00000040  // Informational debug messages
  # DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
  # DEBUG_VARIABLE  0x00000100  // Variable
  # DEBUG_BM        0x00000400  // Boot Manager
  # DEBUG_BLKIO     0x00001000  // BlkIo Driver
  # DEBUG_NET       0x00004000  // SNP Driver
  # DEBUG_UNDI      0x00010000  // UNDI Driver
  # DEBUG_LOADFILE  0x00020000  // LoadFile
  # DEBUG_EVENT     0x00080000  // Event messages
  # DEBUG_GCD       0x00100000  // Global Coherency Database changes
  # DEBUG_CACHE     0x00200000  // Memory range cachability changes
  # DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
  #                             // significantly impact boot performance
  # DEBUG_ERROR     0x80000000  // Error
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x8000004F

!ifdef $(SOURCE_DEBUG_ENABLE)
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x17
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
!endif

  # This PCD is used to set the base address of the PCI express hierarchy. It
  # is only consulted when OVMF runs on Q35. In that case it is programmed into
  # the PCIEXBAR register.
  #
  # On Q35 machine types that QEMU intends to support in the long term, QEMU
  # never lets the RAM below 4 GB exceed 2 GB.
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x80000000

!ifdef $(SOURCE_DEBUG_ENABLE)
  gEfiSourceLevelDebugPkgTokenSpaceGuid.PcdDebugLoadImageMethod|0x2
!endif

[PcdsFixedAtBuild.X64]
!if $(HTTP_BOOT_ENABLE) == TRUE
  gEfiNetworkPkgTokenSpaceGuid.PcdAllowHttpConnections|TRUE
!endif

  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdShellFile|{ 0x83, 0xA5, 0x04, 0x7C, 0x3E, 0x9E, 0x1C, 0x4F, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1 }

!if $(SMM_REQUIRE) == TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x4000
!endif

  # IRQs 5, 9, 10, 11 are level-triggered
  gPcAtChipsetPkgTokenSpaceGuid.Pcd8259LegacyModeEdgeLevel|0x0E20

  # Point to the MdeModulePkg/Application/UiApp/UiApp.inf
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x21, 0xaa, 0x2c, 0x46, 0x14, 0x76, 0x03, 0x45, 0x83, 0x6e, 0x8a, 0xb6, 0xf4, 0x66, 0x23, 0x31 }

################################################################################
#
# Pcd Dynamic Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsDynamicDefault]
  # only set when
  #   ($(SMM_REQUIRE) == FALSE)
  gEfiMdeModulePkgTokenSpaceGuid.PcdEmuVariableNvStoreReserved|0

  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDisableBusEnumeration|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|800
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|600
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiS3Enable|FALSE
  gUefiOvmfPkgTokenSpaceGuid.PcdOvmfHostBridgePciDevId|0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciIoBase|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciIoSize|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciMmio32Base|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciMmio32Size|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciMmio64Base|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdPciMmio64Size|0x800000000

  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|0

  # Set video resolution for text setup.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoHorizontalResolution|640
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupVideoVerticalResolution|480

  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0208
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosDocRev|0x0
  gUefiOvmfPkgTokenSpaceGuid.PcdQemuSmbiosValidated|FALSE

  # Noexec settings for DXE.
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetNxForStack|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPropertiesTableEnable|FALSE

  # UefiCpuPkg PCDs related to initial AP bringup and general AP management.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|64
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApInitTimeOutInMicroSeconds|50000

  # Set memory encryption mask
  gEfiMdeModulePkgTokenSpaceGuid.PcdPteMemoryEncryptionAddressOrMask|0x0

!if $(SMM_REQUIRE) == TRUE
  gUefiOvmfPkgTokenSpaceGuid.PcdQ35TsegMbytes|8
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmSyncMode|0x01
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|100000
!endif

  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x00

!if $(TPM2_ENABLE) == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

################################################################################
#
# Components Section - list of all EDK II Modules needed by this Platform.
#
################################################################################
[Components.IA32]
  OvmfPkg/ResetVector/ResetVector.inf

  #
  # SEC Phase modules
  #
  OvmfPkg/Sec/SecMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

  #
  # PEI Phase modules
  #
  MdeModulePkg/Core/Pei/PeiMain.inf
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf  {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/StatusCodeHandler/Pei/StatusCodeHandlerPei.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf

  OvmfPkg/PlatformPei/PlatformPei.inf
  UefiCpuPkg/Universal/Acpi/S3Resume2Pei/S3Resume2Pei.inf {
    <LibraryClasses>
!if $(SMM_REQUIRE) == TRUE
      LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxPeiLib.inf
!endif
  }
!if $(SMM_REQUIRE) == TRUE
  OvmfPkg/SmmAccess/SmmAccessPei.inf
!endif
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf

!if $(TPM2_ENABLE) == TRUE
  OvmfPkg/Tcg/Tcg2Config/Tcg2ConfigPei.inf
  SecurityPkg/Tcg/Tcg2Pei/Tcg2Pei.inf {
    <LibraryClasses>
      HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterPei.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
  }
!if $(TPM2_CONFIG_ENABLE) == TRUE
  SecurityPkg/Tcg/Tcg2Config/Tcg2ConfigDxe.inf
!endif
!endif

[Components.X64]
  #
  # DXE Phase modules
  #
  MdeModulePkg/Core/Dxe/DxeMain.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  }

  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf  {
   <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }

  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf

  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf {
    <LibraryClasses>
!if $(SECURE_BOOT_ENABLE) == TRUE
      NULL|SecurityPkg/Library/DxeImageVerificationLib/DxeImageVerificationLib.inf
!endif
!if $(TPM2_ENABLE) == TRUE
      NULL|SecurityPkg/Library/DxeTpm2MeasureBootLib/DxeTpm2MeasureBootLib.inf
!endif
  }

  MdeModulePkg/Universal/EbcDxe/EbcDxe.inf
  PcAtChipsetPkg/8259InterruptControllerDxe/8259.inf
  UefiCpuPkg/CpuIo2Dxe/CpuIo2Dxe.inf
  UefiCpuPkg/CpuDxe/CpuDxe.inf
  PcAtChipsetPkg/8254TimerDxe/8254Timer.inf
  OvmfPkg/IncompatiblePciDeviceSupportDxe/IncompatiblePciDeviceSupport.inf
  OvmfPkg/PciHotPlugInitDxe/PciHotPlugInit.inf
  MdeModulePkg/Bus/Pci/PciHostBridgeDxe/PciHostBridgeDxe.inf {
    <LibraryClasses>
      PciHostBridgeLib|OvmfPkg/Library/PciHostBridgeLib/PciHostBridgeLib.inf
      NULL|OvmfPkg/Library/PlatformHasIoMmuLib/PlatformHasIoMmuLib.inf
  }
  MdeModulePkg/Bus/Pci/PciBusDxe/PciBusDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  MdeModulePkg/Universal/Metronome/Metronome.inf
  PcAtChipsetPkg/PcatRealTimeClockRuntimeDxe/PcatRealTimeClockRuntimeDxe.inf
  MdeModulePkg/Universal/DriverHealthManagerDxe/DriverHealthManagerDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf {
    <LibraryClasses>
!ifdef $(CSM_ENABLE)
      NULL|OvmfPkg/Csm/CsmSupportLib/CsmSupportLib.inf
      NULL|IntelFrameworkModulePkg/Library/LegacyBootManagerLib/LegacyBootManagerLib.inf
!endif
  }
  MdeModulePkg/Logo/LogoDxe.inf
  MdeModulePkg/Application/UiApp/UiApp.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/DeviceManagerUiLib/DeviceManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootManagerUiLib/BootManagerUiLib.inf
      NULL|MdeModulePkg/Library/BootMaintenanceManagerUiLib/BootMaintenanceManagerUiLib.inf
!ifdef $(CSM_ENABLE)
      NULL|IntelFrameworkModulePkg/Library/LegacyBootManagerLib/LegacyBootManagerLib.inf
      NULL|IntelFrameworkModulePkg/Library/LegacyBootMaintUiLib/LegacyBootMaintUiLib.inf
!endif
  }
  OvmfPkg/VirtioPciDeviceDxe/VirtioPciDeviceDxe.inf
  OvmfPkg/Virtio10Dxe/Virtio10.inf
  OvmfPkg/VirtioBlkDxe/VirtioBlk.inf
  OvmfPkg/VirtioScsiDxe/VirtioScsi.inf
  OvmfPkg/VirtioRngDxe/VirtioRng.inf
  OvmfPkg/XenIoPciDxe/XenIoPciDxe.inf
  OvmfPkg/XenBusDxe/XenBusDxe.inf
  OvmfPkg/XenPvBlkDxe/XenPvBlkDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/MonotonicCounterRuntimeDxe/MonotonicCounterRuntimeDxe.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  MdeModulePkg/Universal/Console/TerminalDxe/TerminalDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf {
    <LibraryClasses>
      DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  }
  MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/RamDiskDxe/RamDiskDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Universal/Disk/UdfDxe/UdfDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiBusDxe/ScsiBusDxe.inf
  MdeModulePkg/Bus/Scsi/ScsiDiskDxe/ScsiDiskDxe.inf
  OvmfPkg/SataControllerDxe/SataControllerDxe.inf
  MdeModulePkg/Bus/Ata/AtaAtapiPassThru/AtaAtapiPassThru.inf
  MdeModulePkg/Bus/Ata/AtaBusDxe/AtaBusDxe.inf
  MdeModulePkg/Bus/Pci/NvmExpressDxe/NvmExpressDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/SetupBrowserDxe/SetupBrowserDxe.inf
  MdeModulePkg/Universal/DisplayEngineDxe/DisplayEngineDxe.inf
  MdeModulePkg/Universal/MemoryTest/NullMemoryTestDxe/NullMemoryTestDxe.inf

  OvmfPkg/QemuVideoDxe/QemuVideoDxe.inf
  OvmfPkg/QemuRamfbDxe/QemuRamfbDxe.inf
  OvmfPkg/VirtioGpuDxe/VirtioGpu.inf

  #
  # ISA Support
  #
  PcAtChipsetPkg/IsaAcpiDxe/IsaAcpi.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaBusDxe/IsaBusDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaSerialDxe/IsaSerialDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/Ps2KeyboardDxe/Ps2keyboardDxe.inf
  IntelFrameworkModulePkg/Bus/Isa/IsaFloppyDxe/IsaFloppyDxe.inf

  #
  # SMBIOS Support
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf {
    <LibraryClasses>
      NULL|OvmfPkg/Library/SmbiosVersionLib/DetectSmbiosVersionLib.inf
  }
  OvmfPkg/SmbiosPlatformDxe/SmbiosPlatformDxe.inf

  #
  # ACPI Support
  #
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  OvmfPkg/AcpiPlatformDxe/AcpiPlatformDxe.inf
  OvmfPkg/AcpiTables/AcpiTables.inf
  MdeModulePkg/Universal/Acpi/S3SaveStateDxe/S3SaveStateDxe.inf
  MdeModulePkg/Universal/Acpi/BootScriptExecutorDxe/BootScriptExecutorDxe.inf
  MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf

  #
  # Network Support
  #
  MdeModulePkg/Universal/Network/SnpDxe/SnpDxe.inf
  MdeModulePkg/Universal/Network/DpcDxe/DpcDxe.inf
  MdeModulePkg/Universal/Network/MnpDxe/MnpDxe.inf
  MdeModulePkg/Universal/Network/VlanConfigDxe/VlanConfigDxe.inf
  MdeModulePkg/Universal/Network/ArpDxe/ArpDxe.inf
  MdeModulePkg/Universal/Network/Dhcp4Dxe/Dhcp4Dxe.inf
  MdeModulePkg/Universal/Network/Ip4Dxe/Ip4Dxe.inf
  MdeModulePkg/Universal/Network/Mtftp4Dxe/Mtftp4Dxe.inf
  MdeModulePkg/Universal/Network/Udp4Dxe/Udp4Dxe.inf
  NetworkPkg/UefiPxeBcDxe/UefiPxeBcDxe.inf
  NetworkPkg/TcpDxe/TcpDxe.inf
  NetworkPkg/IScsiDxe/IScsiDxe.inf
!if $(NETWORK_IP6_ENABLE) == TRUE
  NetworkPkg/Ip6Dxe/Ip6Dxe.inf
  NetworkPkg/Udp6Dxe/Udp6Dxe.inf
  NetworkPkg/Dhcp6Dxe/Dhcp6Dxe.inf
  NetworkPkg/Mtftp6Dxe/Mtftp6Dxe.inf
!endif
!if $(HTTP_BOOT_ENABLE) == TRUE
  NetworkPkg/DnsDxe/DnsDxe.inf
  NetworkPkg/HttpUtilitiesDxe/HttpUtilitiesDxe.inf
  NetworkPkg/HttpDxe/HttpDxe.inf
  NetworkPkg/HttpBootDxe/HttpBootDxe.inf
!endif
!if $(TLS_ENABLE) == TRUE
  NetworkPkg/TlsDxe/TlsDxe.inf
  NetworkPkg/TlsAuthConfigDxe/TlsAuthConfigDxe.inf {
    <LibraryClasses>
      NULL|OvmfPkg/Library/TlsAuthConfigLib/TlsAuthConfigLib.inf
  }
!endif
  OvmfPkg/VirtioNetDxe/VirtioNet.inf

  #
  # Usb Support
  #
  MdeModulePkg/Bus/Pci/UhciDxe/UhciDxe.inf
  MdeModulePkg/Bus/Pci/EhciDxe/EhciDxe.inf
  MdeModulePkg/Bus/Pci/XhciDxe/XhciDxe.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

!ifdef $(CSM_ENABLE)
  IntelFrameworkModulePkg/Csm/BiosThunk/VideoDxe/VideoDxe.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  }
  IntelFrameworkModulePkg/Csm/LegacyBiosDxe/LegacyBiosDxe.inf
  OvmfPkg/Csm/Csm16/Csm16.inf
!endif

!if $(TOOL_CHAIN_TAG) != "XCODE5"
  ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf {
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }
!endif
  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
!if $(NETWORK_IP6_ENABLE) == TRUE
      NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
!endif
      HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
      PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
      BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
      gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|8000
  }

!if $(SECURE_BOOT_ENABLE) == TRUE
  SecurityPkg/VariableAuthenticated/SecureBootConfigDxe/SecureBootConfigDxe.inf
!endif

  OvmfPkg/PlatformDxe/Platform.inf
  OvmfPkg/AmdSevDxe/AmdSevDxe.inf
  OvmfPkg/IoMmuDxe/IoMmuDxe.inf

!if $(SMM_REQUIRE) == TRUE
  OvmfPkg/SmmAccess/SmmAccess2Dxe.inf
  OvmfPkg/SmmControl2Dxe/SmmControl2Dxe.inf
  UefiCpuPkg/CpuS3DataDxe/CpuS3DataDxe.inf

  #
  # SMM Initial Program Load (a DXE_RUNTIME_DRIVER)
  #
  MdeModulePkg/Core/PiSmmCore/PiSmmIpl.inf

  #
  # SMM_CORE
  #
  MdeModulePkg/Core/PiSmmCore/PiSmmCore.inf

  #
  # Privileged drivers (DXE_SMM_DRIVER modules)
  #
  UefiCpuPkg/CpuIo2Smm/CpuIo2Smm.inf
  MdeModulePkg/Universal/LockBox/SmmLockBox/SmmLockBox.inf {
    <LibraryClasses>
      LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxSmmLib.inf
  }
  UefiCpuPkg/PiSmmCpuDxeSmm/PiSmmCpuDxeSmm.inf {
    <LibraryClasses>
      SmmCpuPlatformHookLib|UefiCpuPkg/Library/SmmCpuPlatformHookLibNull/SmmCpuPlatformHookLibNull.inf
      SmmCpuFeaturesLib|OvmfPkg/Library/SmmCpuFeaturesLib/SmmCpuFeaturesLib.inf
  }

  #
  # Variable driver stack (SMM)
  #
  OvmfPkg/QemuFlashFvbServicesRuntimeDxe/FvbServicesSmm.inf
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteSmm.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmm.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
  }
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableSmmRuntimeDxe.inf

!else

  #
  # Variable driver stack (non-SMM)
  #
  OvmfPkg/QemuFlashFvbServicesRuntimeDxe/FvbServicesRuntimeDxe.inf
  OvmfPkg/EmuVariableFvbRuntimeDxe/Fvb.inf {
    <LibraryClasses>
      PlatformFvbLib|OvmfPkg/Library/EmuVariableFvbLib/EmuVariableFvbLib.inf
  }
  MdeModulePkg/Universal/FaultTolerantWriteDxe/FaultTolerantWriteDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/VarCheckUefiLib/VarCheckUefiLib.inf
  }
!endif

!if $(TPM2_ENABLE) == TRUE
  SecurityPkg/Tcg/Tcg2Dxe/Tcg2Dxe.inf {
    <LibraryClasses>
      Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibRouter/Tpm2DeviceLibRouterDxe.inf
      NULL|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2InstanceLibDTpm.inf
      HashLib|SecurityPkg/Library/HashLibBaseCryptoRouter/HashLibBaseCryptoRouterDxe.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
  }
!endif
