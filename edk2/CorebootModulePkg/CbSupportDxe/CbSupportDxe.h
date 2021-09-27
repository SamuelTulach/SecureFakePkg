/** @file
  The header file of Coreboot Support DXE.

Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#ifndef __DXE_COREBOOT_SUPPORT_H__
#define __DXE_COREBOOT_SUPPORT_H__

#include <PiDxe.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>

#include <Guid/Acpi.h>
#include <Guid/SmBios.h>
#include <Guid/SystemTableInfoGuid.h>
#include <Guid/AcpiBoardInfoGuid.h>
#include <Guid/FrameBufferInfoGuid.h>

#include <IndustryStandard/Acpi.h>

#endif
