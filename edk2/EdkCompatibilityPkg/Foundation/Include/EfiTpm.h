/*++

Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  EfiTpm.h

Abstract:

  EFI definition according to TCG_EFI_Platform_1_20_Final

  See http://trustedcomputinggroup.org for latest specification updates

--*/

#ifndef _EFI_TPM_H_
#define _EFI_TPM_H_

#include "Tiano.h"

//
// The start of TPM return codes
//
#define TPM_BASE                    (EFI_MAX_BIT + (EFI_MAX_BIT >> 1))
#include "Tpm12.h"

//
// Standard event types
//
#define EV_POST_CODE                ((TCG_EVENTTYPE) 0x00000001)
#define EV_SEPARATOR                ((TCG_EVENTTYPE) 0x00000004)
#define EV_S_CRTM_CONTENTS          ((TCG_EVENTTYPE) 0x00000007)
#define EV_S_CRTM_VERSION           ((TCG_EVENTTYPE) 0x00000008)

//
// EFI specific event types
//
#define EV_EFI_EVENT_BASE                   ((TCG_EVENTTYPE) 0x80000000)
#define EV_EFI_VARIABLE_DRIVER_CONFIG       (EV_EFI_EVENT_BASE + 1)
#define EV_EFI_VARIABLE_BOOT                (EV_EFI_EVENT_BASE + 2)
#define EV_EFI_BOOT_SERVICES_APPLICATION    (EV_EFI_EVENT_BASE + 3)
#define EV_EFI_BOOT_SERVICES_DRIVER         (EV_EFI_EVENT_BASE + 4)
#define EV_EFI_RUNTIME_SERVICES_DRIVER      (EV_EFI_EVENT_BASE + 5)
#define EV_EFI_GPT_EVENT                    (EV_EFI_EVENT_BASE + 6)
#define EV_EFI_ACTION                       (EV_EFI_EVENT_BASE + 7)
#define EV_EFI_PLATFORM_FIRMWARE_BLOB       (EV_EFI_EVENT_BASE + 8)
#define EV_EFI_HANDOFF_TABLES               (EV_EFI_EVENT_BASE + 9)

//
// Set structure alignment to 1-byte
//
#pragma pack (1)

typedef UINT32                     TCG_EVENTTYPE;

#define TCG_DIGEST                 TPM_DIGEST
#define TCG_PCRINDEX               TPM_PCRINDEX

//
// TCG_PCR_EVENT
//
typedef struct tdTCG_PCR_EVENT {
  TCG_PCRINDEX                      PCRIndex;  // PCRIndex event extended to
  TCG_EVENTTYPE                     EventType; // TCG EFI event type
  TCG_DIGEST                        Digest;    // Value extended into PCRIndex
  UINT32                            EventSize; // Size of the event data
  UINT8                             Event[1];  // The event data
} TCG_PCR_EVENT;

//
// TCG_PCR_EVENT_HDR
//
typedef struct tdTCG_PCR_EVENT_HDR {
  TCG_PCRINDEX                      PCRIndex;
  TCG_EVENTTYPE                     EventType;
  TCG_DIGEST                        Digest;
  UINT32                            EventSize;
} TCG_PCR_EVENT_HDR;

//
// EFI_PLATFORM_FIRMWARE_BLOB
//
// BlobLength should be of type UINTN but we use UINT64 here
// because PEI is 32-bit while DXE is 64-bit on x64 platforms
//
typedef struct tdEFI_PLATFORM_FIRMWARE_BLOB {
  EFI_PHYSICAL_ADDRESS              BlobBase;
  UINT64                            BlobLength;
} EFI_PLATFORM_FIRMWARE_BLOB;

//
// EFI_IMAGE_LOAD_EVENT
//
// This structure is used in EV_EFI_BOOT_SERVICES_APPLICATION,
// EV_EFI_BOOT_SERVICES_DRIVER and EV_EFI_RUNTIME_SERVICES_DRIVER
//
typedef struct tdEFI_IMAGE_LOAD_EVENT {
  EFI_PHYSICAL_ADDRESS              ImageLocationInMemory;
  UINTN                             ImageLengthInMemory;
  UINTN                             ImageLinkTimeAddress;
  UINTN                             LengthOfDevicePath;
  EFI_DEVICE_PATH_PROTOCOL          DevicePath[1];
} EFI_IMAGE_LOAD_EVENT;

//
// EFI_HANDOFF_TABLE_POINTERS
//
// This structure is used in EV_EFI_HANDOFF_TABLES event to facilitate
// the measurement of given configuration tables.
//
typedef struct tdEFI_HANDOFF_TABLE_POINTERS {
  UINTN                             NumberOfTables;
  EFI_CONFIGURATION_TABLE           TableEntry[1];
} EFI_HANDOFF_TABLE_POINTERS;

//
// EFI_VARIABLE_DATA
//
// This structure serves as the header for measuring variables. The name of the
// variable (in Unicode format) should immediately follow, then the variable
// data.
//
typedef struct tdEFI_VARIABLE_DATA {
  EFI_GUID                          VariableName;
  UINTN                             UnicodeNameLength;
  UINTN                             VariableDataLength;
  CHAR16                            UnicodeName[1];
  INT8                              VariableData[1];  // Driver or platform-specific data
} EFI_VARIABLE_DATA;

//
// Restore original structure alignment
//
#pragma pack ()

#endif  // _EFI_TPM_H_
