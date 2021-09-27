/**
  Implementation of 64-bit swap bytes

  Copyright (c) 2006 - 2007, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "BaseLibInternals.h"

UINT64
EFIAPI
InternalMathSwapBytes64 (
  IN      UINT64                    Operand
  )
{
  _asm {
    mov     eax, dword ptr [Operand + 4]
    mov     edx, dword ptr [Operand + 0]
    bswap   eax
    bswap   edx
  }
}

