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
;   Monitor.Asm
;
; Abstract:
;
;   AsmMonitor function
;
; Notes:
;
;------------------------------------------------------------------------------

    SECTION .text

;------------------------------------------------------------------------------
; UINTN
; EFIAPI
; AsmMonitor (
;   IN      UINTN                     Eax,
;   IN      UINTN                     Ecx,
;   IN      UINTN                     Edx
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmMonitor)
ASM_PFX(AsmMonitor):
    mov     eax, [esp + 4]
    mov     ecx, [esp + 8]
    mov     edx, [esp + 12]
    DB      0xf, 1, 0xc8                ; monitor
    ret

