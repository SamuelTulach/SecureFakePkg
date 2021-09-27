/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  

Module Name:

  ScanMem8Wrapper.c
  
Abstract: 

  ScanMem8() implementation.

--*/

#include "BaseMemoryLibInternal.h"

/**
  Scans a target buffer for an 8-bit value, and returns a pointer to the matching 8-bit value
  in the target buffer.

  This function searches target the buffer specified by Buffer and Length from the lowest
  address to the highest address for an 8-bit value that matches Value.  If a match is found,
  then a pointer to the matching byte in the target buffer is returned.  If no match is found,
  then NULL is returned.  If Length is 0, then NULL is returned.
  If Length > 0 and Buffer is NULL, then ASSERT().
  If Length is greater than (MAX_ADDRESS ? Buffer + 1), then ASSERT(). 

  @param  Buffer      Pointer to the target buffer to scan.
  @param  Length      Number of bytes in Buffer to scan.
  @param  Value       Value to search for in the target buffer.

  @return A pointer to the matching byte in the target buffer or NULL otherwise.

**/
VOID *
EFIAPI
ScanMem8 (
  IN CONST VOID  *Buffer,
  IN UINTN       Length,
  IN UINT8       Value
  )
{
  if (Length == 0) {
    return NULL;
  }
  ASSERT (Buffer != NULL);
  ASSERT ((Length - 1) <= (MAX_ADDRESS - (UINTN)Buffer));
 
  return (VOID*)InternalMemScanMem8 (Buffer, Length, Value);
}
