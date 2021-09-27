/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  MemLibGeneric.c
  
Abstract: 

  Architecture Independent Base Memory Library Implementation.

--*/

#include "BaseMemoryLibInternal.h"

/**
  Fills a target buffer with a 16-bit value, and returns the target buffer.

  @param  Buffer  Pointer to the target buffer to fill.
  @param  Length  Number of bytes in Buffer to fill.
  @param  Value   Value with which to fill Length bytes of Buffer.

  @return Buffer

**/
VOID *
EFIAPI
InternalMemSetMem16 (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length,
  IN      UINT16                    Value
  )
{
  do {
    ((UINT16*)Buffer)[--Length] = Value;
  } while (Length != 0);
  return Buffer;
}

/**
  Fills a target buffer with a 32-bit value, and returns the target buffer.

  @param  Buffer  Pointer to the target buffer to fill.
  @param  Length  Number of bytes in Buffer to fill.
  @param  Value   Value with which to fill Length bytes of Buffer.

  @return Buffer

**/
VOID *
EFIAPI
InternalMemSetMem32 (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length,
  IN      UINT32                    Value
  )
{
  do {
    ((UINT32*)Buffer)[--Length] = Value;
  } while (Length != 0);
  return Buffer;
}

/**
  Fills a target buffer with a 64-bit value, and returns the target buffer.

  @param  Buffer  Pointer to the target buffer to fill.
  @param  Length  Number of bytes in Buffer to fill.
  @param  Value   Value with which to fill Length bytes of Buffer.

  @return Buffer

**/
VOID *
EFIAPI
InternalMemSetMem64 (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length,
  IN      UINT64                    Value
  )
{
  do {
    ((UINT64*)Buffer)[--Length] = Value;
  } while (Length != 0);
  return Buffer;
}

/**
  Set Buffer to 0 for Size bytes.

  @param  Buffer Memory to set.
  @param  Size Number of bytes to set

  @return Buffer

**/
VOID *
EFIAPI
InternalMemZeroMem (
  OUT     VOID                      *Buffer,
  IN      UINTN                     Length
  )
{
  return InternalMemSetMem (Buffer, Length, 0);
}

/**
  Compares two memory buffers of a given length.

  @param  DestinationBuffer First memory buffer
  @param  SourceBuffer      Second memory buffer
  @param  Length            Length of DestinationBuffer and SourceBuffer memory
                            regions to compare. Must be non-zero.

  @retval 0     if MemOne == MemTwo

**/
INTN
EFIAPI
InternalMemCompareMem (
  IN      CONST VOID                *DestinationBuffer,
  IN      CONST VOID                *SourceBuffer,
  IN      UINTN                     Length
  )
{
  while ((--Length != 0) &&
         (*(INT8*)DestinationBuffer == *(INT8*)SourceBuffer)) {
    DestinationBuffer = (INT8*)DestinationBuffer + 1;
    SourceBuffer = (INT8*)SourceBuffer + 1;
  }
  return (INTN)*(UINT8*)DestinationBuffer - (INTN)*(UINT8*)SourceBuffer;
}

/**
  Scans a target buffer for an 8-bit value, and returns a pointer to the
  matching 8-bit value in the target buffer.

  @param  Buffer  Pointer to the target buffer to scan.
  @param  Length  Number of bytes in Buffer to scan. Must be non-zero.
  @param  Value   Value to search for in the target buffer.

  @return Pointer to the first occurrence or NULL if not found.

**/
CONST VOID *
EFIAPI
InternalMemScanMem8 (
  IN      CONST VOID                *Buffer,
  IN      UINTN                     Length,
  IN      UINT8                     Value
  )
{
  CONST UINT8                       *Pointer;

  Pointer = (CONST UINT8*)Buffer;
  do {
    if (*(Pointer++) == Value) {
      return Pointer;
    }
  } while (--Length != 0);
  return NULL;
}

/**
  Scans a target buffer for a 16-bit value, and returns a pointer to the
  matching 16-bit value in the target buffer.

  @param  Buffer  Pointer to the target buffer to scan.
  @param  Length  Number of bytes in Buffer to scan. Must be non-zero.
  @param  Value   Value to search for in the target buffer.

  @return Pointer to the first occurrence or NULL if not found.

**/
CONST VOID *
EFIAPI
InternalMemScanMem16 (
  IN      CONST VOID                *Buffer,
  IN      UINTN                     Length,
  IN      UINT16                    Value
  )
{
  CONST UINT16                      *Pointer;

  Pointer = (CONST UINT16*)Buffer;
  do {
    if (*(Pointer++) == Value) {
      return Pointer;
    }
  } while (--Length != 0);
  return NULL;
}

/**
  Scans a target buffer for a 32-bit value, and returns a pointer to the
  matching 32-bit value in the target buffer.

  @param  Buffer  Pointer to the target buffer to scan.
  @param  Length  Number of bytes in Buffer to scan. Must be non-zero.
  @param  Value   Value to search for in the target buffer.

  @return Pointer to the first occurrence or NULL if not found.

**/
CONST VOID *
EFIAPI
InternalMemScanMem32 (
  IN      CONST VOID                *Buffer,
  IN      UINTN                     Length,
  IN      UINT32                    Value
  )
{
  CONST UINT32                      *Pointer;

  Pointer = (CONST UINT32*)Buffer;
  do {
    if (*(Pointer++) == Value) {
      return Pointer;
    }
  } while (--Length != 0);
  return NULL;
}

/**
  Scans a target buffer for a 64-bit value, and returns a pointer to the
  matching 64-bit value in the target buffer.

  @param  Buffer  Pointer to the target buffer to scan.
  @param  Length  Number of bytes in Buffer to scan. Must be non-zero.
  @param  Value   Value to search for in the target buffer.

  @return Pointer to the first occurrence or NULL if not found.

**/
CONST VOID *
EFIAPI
InternalMemScanMem64 (
  IN      CONST VOID                *Buffer,
  IN      UINTN                     Length,
  IN      UINT64                    Value
  )
{
  CONST UINT64                      *Pointer;

  Pointer = (CONST UINT64*)Buffer;
  do {
    if (*(Pointer++) == Value) {
      return Pointer;
    }
  } while (--Length != 0);
  return NULL;
}
