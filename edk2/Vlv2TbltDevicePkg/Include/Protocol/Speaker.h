/*++

  Copyright (c) 2004  - 2014, Intel Corporation. All rights reserved.<BR>
                                                                                   
  This program and the accompanying materials are licensed and made available under
  the terms and conditions of the BSD License that accompanies this distribution.  
  The full text of the license may be found at                                     
  http://opensource.org/licenses/bsd-license.php.                                  
                                                                                   
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,            
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.    
                                                                                   

Module Name:

  Speaker.h

Abstract:

  EFI Speaker Interface Protocol

Revision History

**/

#ifndef _EFI_SPEAKER_H
#define _EFI_SPEAKER_H

//
// Global Id for Speaker Interface
//
#define EFI_SPEAKER_INTERFACE_PROTOCOL_GUID \
  { \
    0x400b4476, 0x3081, 0x11d6, 0x87, 0xed, 0x00, 0x06, 0x29, 0x45, 0xc3, 0xb9 \
  }

typedef struct _EFI_SPEAKER_IF_PROTOCOL  EFI_SPEAKER_IF_PROTOCOL;

//
// Beep Code
//
typedef
EFI_STATUS
(EFIAPI *EFI_GENERATE_BEEP) (
  IN EFI_SPEAKER_IF_PROTOCOL                * This,
  IN     UINTN                              NumberOfBeep,
  IN     UINTN                              BeepDuration,
  IN     UINTN                              TimeInterval
  );

//
// Set Frequency
//
typedef
EFI_STATUS
(EFIAPI *EFI_SPEAKER_SET_FREQUENCY) (
  IN EFI_SPEAKER_IF_PROTOCOL               * This,
  IN     UINT16                            Frequency
  );

//
// Protocol definition
//
struct _EFI_SPEAKER_IF_PROTOCOL {
  EFI_SPEAKER_SET_FREQUENCY SetSpeakerToneFrequency;
  EFI_GENERATE_BEEP         GenerateBeep;
} ;

extern EFI_GUID gEfiSpeakerInterfaceProtocolGuid;
#endif
