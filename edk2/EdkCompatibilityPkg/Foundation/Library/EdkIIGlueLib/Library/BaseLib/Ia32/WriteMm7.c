/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  WriteMm7.c
  
Abstract: 


--*/

#include "BaseLibInternals.h"

VOID
EFIAPI
AsmWriteMm7 (
  IN UINT64   Value
  )
{
  _asm {
    movq    mm7, qword ptr [Value]
    emms
  }
}

