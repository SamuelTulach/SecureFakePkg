/** @file
  DXE capsule report related function.
  Dummy function for runtime module, because CapsuleDxeRuntime
  does not need record capsule status variable.

  Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Protocol/FirmwareManagement.h>
#include <Guid/FmpCapsule.h>
#include <Library/CapsuleLib.h>

/**
  Record capsule status variable and to local cache.

  @param[in] CapsuleHeader  The capsule image header
  @param[in] CapsuleStatus  The capsule process stauts

  @retval EFI_SUCCESS          The capsule status variable is recorded.
  @retval EFI_OUT_OF_RESOURCES No resource to record the capsule status variable.
**/
EFI_STATUS
RecordCapsuleStatusVariable (
  IN EFI_CAPSULE_HEADER                           *CapsuleHeader,
  IN EFI_STATUS                                   CapsuleStatus
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Record FMP capsule status variable and to local cache.

  @param[in] CapsuleHeader  The capsule image header
  @param[in] CapsuleStatus  The capsule process stauts
  @param[in] PayloadIndex   FMP payload index
  @param[in] ImageHeader    FMP image header
  @param[in] FmpDevicePath  DevicePath associated with the FMP producer

  @retval EFI_SUCCESS          The capsule status variable is recorded.
  @retval EFI_OUT_OF_RESOURCES No resource to record the capsule status variable.
**/
EFI_STATUS
RecordFmpCapsuleStatusVariable (
  IN EFI_CAPSULE_HEADER                            *CapsuleHeader,
  IN EFI_STATUS                                    CapsuleStatus,
  IN UINTN                                         PayloadIndex,
  IN EFI_FIRMWARE_MANAGEMENT_CAPSULE_IMAGE_HEADER  *ImageHeader,
  IN EFI_DEVICE_PATH_PROTOCOL                      *FmpDevicePath OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initialize capsule related variables.
**/
VOID
InitCapsuleVariable (
  VOID
  )
{
  return;
}
