/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  LowBitSet32.c
  
Abstract: 

  Math worker functions.

--*/

#include "BaseLibInternals.h"

/**
  Returns the bit position of the lowest bit set in a 32-bit value.

  This function computes the bit position of the lowest bit set in the 32-bit
  value specified by Operand. If Operand is zero, then -1 is returned.
  Otherwise, a value between 0 and 31 is returned.

  @param  Operand The 32-bit operand to evaluate.

  @return Position of the lowest bit set in Operand if found.
  @retval -1 Operand is zero.

**/
INTN
EFIAPI
LowBitSet32 (
  IN      UINT32                    Operand
  )
{
  INTN                              BitIndex;

  if (Operand == 0) {
    return -1;
  }

  for (BitIndex = 0; (Operand & 1) == 0; BitIndex++, Operand >>= 1);
  return BitIndex;
}
