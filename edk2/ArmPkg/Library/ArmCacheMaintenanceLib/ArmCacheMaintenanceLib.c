/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2011 - 2014, ARM Limited. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include <Base.h>
#include <Library/ArmLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

STATIC
VOID
CacheRangeOperation (
  IN  VOID            *Start,
  IN  UINTN           Length,
  IN  LINE_OPERATION  LineOperation,
  IN  UINTN           LineLength
  )
{
  UINTN ArmCacheLineAlignmentMask  = LineLength - 1;

  // Align address (rounding down)
  UINTN AlignedAddress = (UINTN)Start - ((UINTN)Start & ArmCacheLineAlignmentMask);
  UINTN EndAddress     = (UINTN)Start + Length;

  // Perform the line operation on an address in each cache line
  while (AlignedAddress < EndAddress) {
    LineOperation(AlignedAddress);
    AlignedAddress += LineLength;
  }
  ArmDataSynchronizationBarrier ();
}

VOID
EFIAPI
InvalidateInstructionCache (
  VOID
  )
{
  ASSERT (FALSE);
}

VOID
EFIAPI
InvalidateDataCache (
  VOID
  )
{
  ASSERT (FALSE);
}

VOID *
EFIAPI
InvalidateInstructionCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  CacheRangeOperation (Address, Length, ArmCleanDataCacheEntryToPoUByMVA,
    ArmDataCacheLineLength ());
  CacheRangeOperation (Address, Length,
    ArmInvalidateInstructionCacheEntryToPoUByMVA,
    ArmInstructionCacheLineLength ());

  ArmInstructionSynchronizationBarrier ();

  return Address;
}

VOID
EFIAPI
WriteBackInvalidateDataCache (
  VOID
  )
{
  ASSERT (FALSE);
}

VOID *
EFIAPI
WriteBackInvalidateDataCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  CacheRangeOperation(Address, Length, ArmCleanInvalidateDataCacheEntryByMVA,
    ArmDataCacheLineLength ());
  return Address;
}

VOID
EFIAPI
WriteBackDataCache (
  VOID
  )
{
  ASSERT (FALSE);
}

VOID *
EFIAPI
WriteBackDataCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  CacheRangeOperation(Address, Length, ArmCleanDataCacheEntryByMVA,
    ArmDataCacheLineLength ());
  return Address;
}

VOID *
EFIAPI
InvalidateDataCacheRange (
  IN      VOID                      *Address,
  IN      UINTN                     Length
  )
{
  CacheRangeOperation(Address, Length, ArmInvalidateDataCacheEntryByMVA,
    ArmDataCacheLineLength ());
  return Address;
}
