/** @file
Protocol used for Platform Policy definition.

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


**/

#ifndef _PLATFORM_POLICY_H_
#define _PLATFORM_POLICY_H_

typedef struct _EFI_PLATFORM_POLICY_PROTOCOL EFI_PLATFORM_POLICY_PROTOCOL;

#define EFI_PLATFORM_POLICY_PROTOCOL_GUID \
  { \
    0x2977064f, 0xab96, 0x4fa9, { 0x85, 0x45, 0xf9, 0xc4, 0x02, 0x51, 0xe0, 0x7f } \
  }

//
// Protocol to describe various platform information. Add to this as needed.
//
struct _EFI_PLATFORM_POLICY_PROTOCOL {
  UINT8 NumRsvdSmbusAddresses;
  UINT8 *RsvdSmbusAddresses;
};

extern EFI_GUID gEfiPlatformPolicyProtocolGuid;

#endif
