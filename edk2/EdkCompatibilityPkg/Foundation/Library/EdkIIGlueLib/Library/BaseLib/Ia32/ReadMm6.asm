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
;   ReadMm6.Asm
;
; Abstract:
;
;   AsmReadMm6 function
;
; Notes:
;
;------------------------------------------------------------------------------

    .586
    .model  flat,C
    .mmx
    .code

;------------------------------------------------------------------------------
; UINT64
; EFIAPI
; AsmReadMm6 (
;   VOID
;   );
;------------------------------------------------------------------------------
AsmReadMm6  PROC
    push    eax
    push    eax
    movq    [esp], mm6
    pop     eax
    pop     edx
    ret
AsmReadMm6  ENDP

    END
