;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   ScanMem16.Asm
;
; Abstract:
;
;   ScanMem16 function
;
; Notes:
;
;   The following BaseMemoryLib instances share the same version of this file:
;
;       BaseMemoryLibRepStr
;       BaseMemoryLibMmx
;       BaseMemoryLibSse2
;
;------------------------------------------------------------------------------

    .686
    .model  flat,C
    .code

;------------------------------------------------------------------------------
; CONST VOID *
; EFIAPI
; InternalMemScanMem16 (
;   IN      CONST VOID                *Buffer,
;   IN      UINTN                     Length,
;   IN      UINT16                    Value
;   );
;------------------------------------------------------------------------------
InternalMemScanMem16    PROC    USES    edi
    mov     ecx, [esp + 12]
    mov     edi, [esp + 8]
    mov     eax, [esp + 16]
    repne   scasw
    lea     eax, [edi - 2]
    cmovnz  eax, ecx
    ret
InternalMemScanMem16    ENDP

    END
