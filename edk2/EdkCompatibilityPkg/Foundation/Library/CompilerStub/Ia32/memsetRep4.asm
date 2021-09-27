;------------------------------------------------------------------------------
;
; Copyright (c) 2007, Intel Corporation. All rights reserved.<BR>
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
;   SetMem.asm
;
; Abstract:
;
;   memset function
;
; Notes:
;
;------------------------------------------------------------------------------

    .686
    .model  flat,C
    .code

;------------------------------------------------------------------------------
;  VOID *
;  memset (
;    OUT VOID   *Buffer,
;    IN  UINT8  Value,
;    IN  UINTN  Count
;    )
;------------------------------------------------------------------------------
memset   PROC    USES    edi
    mov         ecx, [esp + 16]
    test        ecx, ecx
    je          @F
    mov         al,  [esp + 12]
    mov         ah,  al
    shrd        edx, eax, 16
    shld        eax, edx, 16
    mov         edx, ecx
    mov         edi, [esp + 8]
    shr         ecx, 2
    rep stosd
    mov         ecx, edx
    and         ecx, 3
    rep stosb
@@:
    mov         eax, [esp + 8]
    ret
memset   ENDP

    END
