/** @file
  This Library uses Framework RecoveryModule PPI to do system recovery.

  This library instance is no longer used and module using this library
  class should update to directly locate EFI_PEI_RECOVERY_MODULE_PPI defined
  in PI 1.2 specification.

Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>

#include <Ppi/RecoveryModule.h>

/**
  Calling this function causes the system do recovery.

  @retval EFI_SUCESS   Sucess to do recovery.
  @retval Others       Fail to do recovery.
**/
EFI_STATUS
EFIAPI
PeiRecoverFirmware (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_RECOVERY_MODULE_PPI     *PeiRecovery;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRecoveryModulePpiGuid,
             0,
             NULL,
             (VOID **)&PeiRecovery
             );
  ASSERT_EFI_ERROR (Status);

  return PeiRecovery->LoadRecoveryCapsule ((EFI_PEI_SERVICES  **) GetPeiServicesTablePointer(), PeiRecovery);
}

