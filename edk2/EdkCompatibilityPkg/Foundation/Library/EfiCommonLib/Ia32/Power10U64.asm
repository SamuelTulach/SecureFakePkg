;/*++
;
;Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
;This program and the accompanying materials                          
;are licensed and made available under the terms and conditions of the BSD License         
;which accompanies this distribution.  The full text of the license may be found at        
;http://opensource.org/licenses/bsd-license.php                                            
;                                                                                          
;THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
;WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             
;
;Module Name:
;
;  Power10U64.c
;
;Abstract:
;
;  Calculates Operand * 10 ^ Power
;
;--*/
;
;#include "Tiano.h"
;---------------------------------------------------------------------------
    .686
    .model  flat,C
    .code

MultU64x32    PROTO   C
;---------------------------------------------------------------------------
;
;UINT64
;MultU64x32 (
;  IN UINT64   Multiplicand,
;  IN UINTN    Multiplier
;  );
;
;UINT64
;Power10U64 (
;  IN UINT64   Operand,
;  IN UINTN    Power
;  )
;/*++
;
;Routine Description:
;
;  Raise 10 to the power of Power, and multiply the result with Operand
;
;Arguments:
;
;  Operand  - multiplicand
;  Power    - power
;
;Returns:
;
;  Operand * 10 ^ Power
;
;--*/
Power10U64 PROC
  push   ebp
  mov    ebp, esp
  mov    eax, dword ptr [ebp + 8]; dword ptr Operand[0]
  mov    edx, dword ptr [ebp + 0Ch]; dword ptr Operand[4]
  mov    ecx, dword ptr [ebp + 10h] ;Power
  jcxz   _Power10U64_Done
  
_Power10U64_Wend:
  push   ecx
  push   10
  push   [ebp + 0Ch]; dword ptr Operand[4]
  push   [ebp + 8]; dword ptr Operand[0]
  call   MultU64x32
  add    esp, 0cH
  pop    ecx
  mov    [ebp + 8] , eax; dword ptr Operand[0]
  mov    [ebp + 0Ch] , edx; dword ptr Operand[4]
  loop   _Power10U64_Wend

_Power10U64_Done:

  pop    ebp
  ret
Power10U64 ENDP
	END
