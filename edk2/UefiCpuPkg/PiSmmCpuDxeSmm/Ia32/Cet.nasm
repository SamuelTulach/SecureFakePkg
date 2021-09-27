;------------------------------------------------------------------------------ ;
; Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php.
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
;-------------------------------------------------------------------------------

%include "Nasm.inc"

SECTION .text

global ASM_PFX(DisableCet)
ASM_PFX(DisableCet):

    ; Skip the pushed data for call
    mov     eax, 1
    INCSSP_EAX

    mov     eax, cr4
    btr     eax, 23                      ; clear CET
    mov     cr4, eax
    ret

global ASM_PFX(EnableCet)
ASM_PFX(EnableCet):

    mov     eax, cr4
    bts     eax, 23                      ; set CET
    mov     cr4, eax

    ; use jmp to skip the check for ret
    pop     eax
    jmp     eax

