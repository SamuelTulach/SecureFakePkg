/** @file
  64-bit Math Worker Function.
  The 32-bit versions of C compiler generate calls to library routines
  to handle 64-bit math. These functions use non-standard calling conventions.

  Copyright (c) 2009 - 2011, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/BaseLib.h>


/*
 * Divides a 64-bit unsigned value by another 64-bit unsigned value and returns
 * the 64-bit unsigned remainder.
 */
__declspec(naked) void __cdecl _aullrem(void)
{
  //
  // Wrapper Implementation over EDKII DivU64x64Remainder() routine
  //    UINT64
  //    EFIAPI
  //    DivU64x64Remainder (
  //      IN      UINT64     Dividend,
  //      IN      UINT64     Divisor,
  //      OUT     UINT64     *Remainder  OPTIONAL
  //      )
  //
  _asm {
    ; Original local stack when calling _aullrem
    ;               -----------------
    ;               |               |
    ;               |---------------|
    ;               |               |
    ;               |--  Divisor  --|
    ;               |               |
    ;               |---------------|
    ;               |               |
    ;               |--  Dividend --|
    ;               |               |
    ;               |---------------|
    ;               |  ReturnAddr** |
    ;       ESP---->|---------------|
    ;

    ;
    ; Set up the local stack for Reminder pointer
    ;
    sub  esp, 8
    push esp

    ;
    ; Set up the local stack for Divisor parameter
    ;
    mov  eax, [esp + 28]
    push eax
    mov  eax, [esp + 28]
    push eax

    ;
    ; Set up the local stack for Dividend parameter
    ;
    mov  eax, [esp + 28]
    push eax
    mov  eax, [esp + 28]
    push eax

    ;
    ; Call native DivU64x64Remainder of BaseLib
    ;
    call DivU64x64Remainder

    ;
    ; Put the Reminder in EDX:EAX as return value
    ;
    mov  eax, [esp + 20]
    mov  edx, [esp + 24]

    ;
    ; Adjust stack
    ;
    add  esp, 28

    ret  16
  }
}
