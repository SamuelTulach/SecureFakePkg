/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  LRotU64.c
  
Abstract: 

  

--*/

#include "BaseLibInternals.h"

UINT64
EFIAPI
InternalMathLRotU64 (
  IN      UINT64                    Operand,
  IN      UINTN                     Count
  )
{
  _asm {
    mov     cl, byte ptr [Count]
    mov     edx, dword ptr [Operand + 4]
    mov     eax, dword ptr [Operand + 0]
    shld    ebx, edx, cl
    shld    edx, eax, cl
    ror     ebx, cl
    shld    eax, ebx, cl
    test    cl, 32                      ; Count >= 32?
    cmovnz  ecx, eax
    cmovnz  eax, edx
    cmovnz  edx, ecx
  }
}

