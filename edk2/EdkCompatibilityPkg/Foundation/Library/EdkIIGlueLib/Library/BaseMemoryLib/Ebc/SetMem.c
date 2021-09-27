/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  SetMem.c
  
Abstract: 

  Internal SetMem

--*/

#include "BaseMemoryLibInternal.h"

/**
  Set Buffer to Value for Size bytes.

  @param  Buffer Memory to set.
  @param  Size Number of bytes to set
  @param  Value Value of the set operation.

  @return Buffer

**/
VOID *
EFIAPI
InternalMemSetMem (
  IN      VOID                      *Buffer,
  IN      UINTN                     Size,
  IN      UINT8                     Value
  )
{
  //
  // Declare the local variables that actually move the data elements as
  // volatile to prevent the optimizer from replacing this function with
  // the intrinsic memset()
  //
  volatile UINT8                    *Pointer;

  Pointer = (UINT8*)Buffer;
  while (Size-- != 0) {
    *(Pointer++) = Value;
  }
  return Buffer;
}
