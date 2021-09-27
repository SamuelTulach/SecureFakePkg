;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
; Module Name:
;
;   SetMem16.nasm
;
; Abstract:
;
;   SetMem16 function
;
; Notes:
;
;------------------------------------------------------------------------------

    SECTION .text

;------------------------------------------------------------------------------
;  VOID *
;  EFIAPI
;  InternalMemSetMem16 (
;    IN VOID   *Buffer,
;    IN UINTN  Count,
;    IN UINT16 Value
;    )
;------------------------------------------------------------------------------
global ASM_PFX(InternalMemSetMem16)
ASM_PFX(InternalMemSetMem16):
    push    edi
    mov     eax, [esp + 16]
    shrd    edx, eax, 16
    shld    eax, edx, 16
    mov     edx, [esp + 12]
    mov     edi, [esp + 8]
    mov     ecx, edx
    and     edx, 3
    shr     ecx, 2
    jz      @SetWords
    movd    mm0, eax
    movd    mm1, eax
    psllq   mm0, 32
    por     mm0, mm1
.0:
    movq    [edi], mm0
    add     edi, 8
    loop    .0
@SetWords:
    mov     ecx, edx
    rep     stosw
    mov     eax, [esp + 8]
    pop     edi
    ret

