/*++

Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  

Module Name:

  CompareMemWrapper.c
  
Abstract: 
  
  CompareMem() implementation.

--*/

#include "BaseMemoryLibInternal.h"

/**
  Compares the contents of two buffers.

  This function compares Length bytes of SourceBuffer to Length bytes of DestinationBuffer.
  If all Length bytes of the two buffers are identical, then 0 is returned.  Otherwise, the
  value returned is the first mismatched byte in SourceBuffer subtracted from the first
  mismatched byte in DestinationBuffer.
  If Length > 0 and DestinationBuffer is NULL and Length > 0, then ASSERT().
  If Length > 0 and SourceBuffer is NULL and Length > 0, then ASSERT().
  If Length is greater than (MAX_ADDRESS - DestinationBuffer + 1), then ASSERT(). 
  If Length is greater than (MAX_ADDRESS - SourceBuffer + 1), then ASSERT(). 


  @param  DestinationBuffer Pointer to the destination buffer to compare.
  @param  SourceBuffer      Pointer to the source buffer to compare.
  @param  Length            Number of bytes to compare.

  @return 0                 All Length bytes of the two buffers are identical.
  @retval Non-zero          The first mismatched byte in SourceBuffer subtracted from the first
                            mismatched byte in DestinationBuffer.

**/
INTN
EFIAPI
GlueCompareMem (
  IN CONST VOID  *DestinationBuffer,
  IN CONST VOID  *SourceBuffer,
  IN UINTN       Length
  )
{
  if (Length == 0 || DestinationBuffer == SourceBuffer) {
    return 0;
  }
  ASSERT (DestinationBuffer != NULL);
  ASSERT (SourceBuffer != NULL);
  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)DestinationBuffer));
  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)SourceBuffer));

  return InternalMemCompareMem (DestinationBuffer, SourceBuffer, Length);
}
