/*++

Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:
  
    AcpiDescription.c
    
Abstract:


   GUIDs used for ACPI Description

--*/

#include "Tiano.h"
#include EFI_GUID_DEFINITION (AcpiDescription)

EFI_GUID  gEfiAcpiDescriptionGuid = EFI_ACPI_DESCRIPTION_GUID;

EFI_GUID_STRING(&gEfiAcpiDescriptionGuid, "ACPI Description", "ACPI Description GUID");
