/** @file
  This GUID is installed to the device handler to specify that the device is a StdErr device.


Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __STANDARD_ERROR_DEVICE_H__
#define __STANDARD_ERROR_DEVICE_H__

#define EFI_STANDARD_ERROR_DEVICE_GUID    \
    { 0xd3b36f2d, 0xd551, 0x11d4, {0x9a, 0x46, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

extern EFI_GUID gEfiStandardErrorDeviceGuid;

#endif
