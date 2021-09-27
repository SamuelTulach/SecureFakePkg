/*++

Copyright (c) 2007, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

Module Name:

  HardwareErrorVariable.c

Abstract:

  GUID for hardware error record variables. Defined in UEFI 2.1.

--*/

#include "EfiSpec.h"
#include EFI_GUID_DEFINITION (HardwareErrorVariable)

EFI_GUID  gEfiHardwareErrorVariableGuid = EFI_HARDWARE_ERROR_VARIABLE_GUID;

EFI_GUID_STRING(&gEfiHardwareErrorVariableGuid, "HwErr", "Hardware Error Variable GUID")
