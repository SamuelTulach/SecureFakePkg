/** @file
Header file for the PCH SPI Runtime Driver.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_SPI_H_
#define _PCH_SPI_H_

#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Spi.h>
#include "SpiCommon.h"
#include <Library/PciExpressLib.h>
#include <IntelQNCRegs.h>
#include <Library/IntelQNCLib.h>
#include <Library/QNCAccessLib.h>
#include <Library/TimerLib.h>

#define EFI_INTERNAL_POINTER  0x00000004


//
// Function prototypes used by the SPI protocol.
//
VOID
PchSpiVirtualddressChangeEvent (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
/*++

Routine Description:

  Fixup internal data pointers so that the services can be called in virtual mode.

Arguments:

  Event     The event registered.
  Context   Event context. Not used in this event handler.

Returns:

  None.

--*/
;

VOID
EFIAPI
SpiPhaseInit (
  VOID
  )
/*++
Routine Description:

  This function is a hook for Spi Dxe phase specific initialization

Arguments:

  None

Returns:

  None

--*/
;
#endif
