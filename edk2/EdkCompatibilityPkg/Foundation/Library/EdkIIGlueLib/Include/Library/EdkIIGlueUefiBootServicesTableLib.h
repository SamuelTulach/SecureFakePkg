/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  EdkIIGlueUefiBootServicesTableLib.h
  
Abstract: 

  Public header file for UEFI Boot Services Table Lib

--*/

#ifndef __EDKII_GLUE_UEFI_BOOT_SERVICES_TABLE_LIB_H__
#define __EDKII_GLUE_UEFI_BOOT_SERVICES_TABLE_LIB_H__

//
// Cache the Image Handle
//
extern EFI_HANDLE         gImageHandle;

//
// Cache pointer to the EFI System Table
//
extern EFI_SYSTEM_TABLE   *gST;

//
// Cache pointer to the EFI Boot Services Table
//
extern EFI_BOOT_SERVICES  *gBS;

#endif
