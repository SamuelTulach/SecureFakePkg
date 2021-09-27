;------------------------------------------------------------------------------ ;
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
;   CpuBreakpoint.Asm
;
; Abstract:
;
;   CpuBreakpoint function
;
; Notes:
;
;------------------------------------------------------------------------------

    .686
    .model  flat
    .xmm
    .code

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; CpuBreakpoint (
;   VOID
;   );
;------------------------------------------------------------------------------
_CpuBreakpoint   PROC
    int  3
    ret
_CpuBreakpoint   ENDP

    END
