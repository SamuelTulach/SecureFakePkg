/*++

Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  DevicePathFromText.c

Abstract:

  DevicePathFromText protocol as defined in the UEFI 2.0 specification.

--*/

#include "EfiSpec.h"
#include EFI_PROTOCOL_DEFINITION (DevicePathFromText)

EFI_GUID gEfiDevicePathFromTextProtocolGuid = EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL_GUID;

EFI_GUID_STRING(&gEfiDevicePathFromTextProtocolGuid, "Device Path From Text Protocol", "UEFI 2.0 Device Path From Text Protocol");
