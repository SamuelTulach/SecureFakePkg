/**
  AsmCpuid function.

  Copyright (c) 2006 - 2007, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "BaseLibInternals.h"

UINT32
EFIAPI
AsmCpuid (
  IN      UINT32                    Index,
  OUT     UINT32                    *RegisterEax,  OPTIONAL
  OUT     UINT32                    *RegisterEbx,  OPTIONAL
  OUT     UINT32                    *RegisterEcx,  OPTIONAL
  OUT     UINT32                    *RegisterEdx   OPTIONAL
  )
{
  _asm {
    mov     eax, Index
    cpuid
    push    ecx
    mov     ecx, RegisterEax
    jecxz   SkipEax
    mov     [ecx], eax
SkipEax:
    mov     ecx, RegisterEbx
    jecxz   SkipEbx
    mov     [ecx], ebx
SkipEbx:
    pop     eax
    mov     ecx, RegisterEcx
    jecxz   SkipEcx
    mov     [ecx], eax
SkipEcx:
    mov     ecx, RegisterEdx
    jecxz   SkipEdx
    mov     [ecx], edx
SkipEdx:
    mov     eax, Index
  }
}

