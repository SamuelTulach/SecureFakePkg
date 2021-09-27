/** @file
  Get TPM 2.0 physical presence information.

  This library will get TPM 2.0 physical presence information.

Copyright (c) 2015 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>

#include <Guid/Tcg2PhysicalPresenceData.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h>

/**
  Return TPM2 ManagementFlags set by PP interface.

  @retval    ManagementFlags    TPM2 Management Flags.
**/
UINT32
EFIAPI
Tcg2PhysicalPresenceLibGetManagementFlags (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariablePpi;
  EFI_TCG2_PHYSICAL_PRESENCE_FLAGS  PpiFlags;
  UINTN                             DataSize;

  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &VariablePpi);
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (EFI_TCG2_PHYSICAL_PRESENCE_FLAGS);
  Status = VariablePpi->GetVariable (
                          VariablePpi,
                          TCG2_PHYSICAL_PRESENCE_FLAGS_VARIABLE,
                          &gEfiTcg2PhysicalPresenceGuid,
                          NULL,
                          &DataSize,
                          &PpiFlags
                          );
  if (EFI_ERROR (Status)) {
    PpiFlags.PPFlags = TCG2_BIOS_TPM_MANAGEMENT_FLAG_DEFAULT | TCG2_BIOS_STORAGE_MANAGEMENT_FLAG_DEFAULT;
  }
  return PpiFlags.PPFlags;
}
