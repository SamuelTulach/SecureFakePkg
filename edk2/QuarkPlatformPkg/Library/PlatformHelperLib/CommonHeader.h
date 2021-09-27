/** @file
Common header file shared by all source files in this component.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/SerialPortLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CapsuleLib.h>
#include <Library/IntelQNCLib.h>
#include <Platform.h>
#include <PlatformBoards.h>
#include <Pcal9555.h>
#include <QNCAccess.h>
#include <Library/QNCAccessLib.h>
#include <IohAccess.h>

#include <Library/PlatformHelperLib.h>

//
// Routines shared between souce modules in this component.
//

EFI_STATUS
WriteFirstFreeSpiProtect (
  IN CONST UINT32                         PchRootComplexBar,
  IN CONST UINT32                         DirectValue,
  IN CONST UINT32                         BaseAddress,
  IN CONST UINT32                         Length,
  OUT UINT32                              *OffsetPtr
  );

VOID
Pcal9555SetPortRegBit (
  IN CONST UINT32                         Pcal9555SlaveAddr,
  IN CONST UINT32                         GpioNum,
  IN CONST UINT8                          RegBase,
  IN CONST BOOLEAN                        LogicOne
  );

#endif
