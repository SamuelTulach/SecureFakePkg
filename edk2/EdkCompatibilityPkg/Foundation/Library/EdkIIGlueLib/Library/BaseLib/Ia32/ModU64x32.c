/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  ModU64x32.c
  
Abstract: 

  

--*/

#include "BaseLibInternals.h"

UINT32
EFIAPI
InternalMathModU64x32 (
  IN      UINT64                    Dividend,
  IN      UINT32                    Divisor
  )
{
  _asm {
    mov     eax, dword ptr [Dividend + 4]
    mov     ecx, Divisor
    xor     edx, edx
    div     ecx
    mov     eax, dword ptr [Dividend + 0]
    div     ecx
    mov     eax, edx
  }
}
