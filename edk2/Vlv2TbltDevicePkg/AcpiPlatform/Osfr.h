/*++

  Copyright (c) 2004  - 2014, Intel Corporation. All rights reserved.<BR>
                                                                                   
  This program and the accompanying materials are licensed and made available under
  the terms and conditions of the BSD License that accompanies this distribution.  
  The full text of the license may be found at                                     
  http://opensource.org/licenses/bsd-license.php.                                  
                                                                                   
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,            
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.    
                                                                                   

Module Name:

  Osfr.h

Abstract:

  This file describes the contents of the ACPI OSFR Table.

--*/

#ifndef _OSFR_H
#define _OSFR_H

//
// Statements that include other files.
//
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>

#pragma pack (1)

#define EFI_ACPI_OSFR_TABLE_REVISION            0x1
//#define EFI_ACPI_OSFR_TABLE_SIGNATURE           'RFSO'
#define EFI_ACPI_OSFR_TABLE_SIGNATURE           SIGNATURE_32('O', 'S', 'F', 'R')  //'RFSO'

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER          Header;
  UINT32                               ObjectCount;
  UINT32                               TableDWORDs [64];
} EFI_ACPI_OSFR_TABLE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER          Header;
  UINT32                               ObjectCount;
} EFI_ACPI_OSFR_TABLE_FIXED_PORTION;

typedef struct {
  EFI_GUID  ObjectUUID;
  UINT32    Reserved1;
  UINT32    ManufacturerNameStringOffset;
  UINT32    ModelNameStringOffset;
  UINT32    Reserved2;
  UINT32    MicrosoftReferenceOffset;
} EFI_ACPI_OSFR_OCUR_OBJECT;

#pragma pack ()

#endif
