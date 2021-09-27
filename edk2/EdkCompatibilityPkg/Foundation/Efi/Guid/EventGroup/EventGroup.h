/** @file
	GUIDs for gBS->CreateEventEx Event Groups. Defined in UEFI 2.0.

	Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
	are licensed and made available under the terms and conditions of the BSD License         
	which accompanies this distribution.  The full text of the license may be found at        
	http://opensource.org/licenses/bsd-license.php                                            

	THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
	WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

	Module Name:	EventGroup.h

**/

#ifndef __EVENT_GROUP_GUID__
#define __EVENT_GROUP_GUID__

                                             
#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES \
  { 0x27abf055, 0xb1b8, 0x4c26, { 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf } }

extern EFI_GUID gEfiEventExitBootServicesGuid;


#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE \
  { 0x13fa7698, 0xc831, 0x49c7, { 0x87, 0xea, 0x8f, 0x43, 0xfc, 0xc2, 0x51, 0x96 } }

extern EFI_GUID gEfiEventVirtualAddressChangeGuid;


#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE \
  { 0x78bee926, 0x692f, 0x48fd, { 0x9e, 0xdb, 0x1, 0x42, 0x2e, 0xf0, 0xd7, 0xab } }

extern EFI_GUID gEfiEventMemoryMapChangeGuid;


#define EFI_EVENT_GROUP_READY_TO_BOOT \
  { 0x7ce88fb3, 0x4bd7, 0x4679, { 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0x0d, 0x2b } }

extern EFI_GUID gEfiEventReadyToBootGuid;


#endif
