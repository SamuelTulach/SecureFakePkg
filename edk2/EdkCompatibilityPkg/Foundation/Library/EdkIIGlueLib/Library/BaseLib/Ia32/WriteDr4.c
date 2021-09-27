/**
  AsmWriteDr4 function

  Copyright (c) 2006 - 2007, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "BaseLibInternals.h"

UINTN
EFIAPI
AsmWriteDr4 (
  IN UINTN Value
  )
{
  _asm {
    mov     eax, Value
    _emit   0x0f         // mov dr4, eax
    _emit   0x23
    _emit   0xe0
  }
}

