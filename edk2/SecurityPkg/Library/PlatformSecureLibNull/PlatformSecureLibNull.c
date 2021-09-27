/** @file
  NULL PlatformSecureLib instance does NOT really detect whether a physical present
  user exists but return TRUE directly. This instance can be used to verify security
  related features during platform enabling and development. It should be replaced
  by a platform-specific method(e.g. Button pressed) in a real platform for product.

Copyright (c) 2011 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi/UefiBaseType.h>

BOOLEAN       mUserPhysicalPresence      = FALSE;

/**

  This function provides a platform-specific method to detect whether the platform
  is operating by a physically present user.

  Programmatic changing of platform security policy (such as disable Secure Boot,
  or switch between Standard/Custom Secure Boot mode) MUST NOT be possible during
  Boot Services or after exiting EFI Boot Services. Only a physically present user
  is allowed to perform these operations.

  NOTE THAT: This function cannot depend on any EFI Variable Service since they are
  not available when this function is called in AuthenticateVariable driver.

  @retval  TRUE       The platform is operated by a physically present user.
  @retval  FALSE      The platform is NOT operated by a physically present user.

**/
BOOLEAN
EFIAPI
UserPhysicalPresent (
  VOID
  )
{
  return mUserPhysicalPresence;
}


/**
  Save user physical presence state from a PCD to mUserPhysicalPresence.

  @retval  EFI_SUCCESS          PcdUserPhysicalPresence is got successfully.

**/
RETURN_STATUS
EFIAPI
PlatformSecureLibNullConstructor (
  VOID
  )
{

  mUserPhysicalPresence = PcdGetBool(PcdUserPhysicalPresence);

  return RETURN_SUCCESS;
}
