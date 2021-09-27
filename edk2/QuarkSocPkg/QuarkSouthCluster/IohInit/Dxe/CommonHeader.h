/** @file
Common header file shared by all source files.

This file includes package header files, library classes and protocol, PPI & GUID definitions.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#ifndef __COMMON_HEADER_H_
#define __COMMON_HEADER_H_

//
// The package level header files this module uses
//
#include <PiDxe.h>
#include <Ioh.h>
#include <IohCommonDefinitions.h>

//
// The protocols, PPI and GUID defintions for this module
//
#include <Protocol/PciRootBridgeIo.h>

#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>

//
// The Library classes this module consumes
//
#include <Library/BaseLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/S3PciLib.h>
#include <Library/S3IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/DevicePathLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IohLib.h>

#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Pci.h>

#endif
