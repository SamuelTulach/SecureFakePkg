/*++

Copyright (c) 1999 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.


Module Name:

  Legacy8259.h
    
Abstract:

  This protocol abstracts the 8259 interrupt controller. This includes
  PCI IRQ routing need to program the PCI Interrupt Line register.

Revision History

  The EFI Legacy 8259 Protocol is compliant with CSM spec 0.96.

--*/

#ifndef _EFI_LEGACY_8259_H
#define _EFI_LEGACY_8259_H

#define EFI_LEGACY_8259_PROTOCOL_GUID \
  { \
    0x38321dba, 0x4fe0, 0x4e17, {0x8a, 0xec, 0x41, 0x30, 0x55, 0xea, 0xed, 0xc1} \
  }

EFI_FORWARD_DECLARATION (EFI_LEGACY_8259_PROTOCOL);

typedef enum {
  Efi8259Irq0,
  Efi8259Irq1,
  Efi8259Irq2,
  Efi8259Irq3,
  Efi8259Irq4,
  Efi8259Irq5,
  Efi8259Irq6,
  Efi8259Irq7,
  Efi8259Irq8,
  Efi8259Irq9,
  Efi8259Irq10,
  Efi8259Irq11,
  Efi8259Irq12,
  Efi8259Irq13,
  Efi8259Irq14,
  Efi8259Irq15,
  Efi8259IrqMax
} EFI_8259_IRQ;

typedef enum {
  Efi8259LegacyMode,
  Efi8259ProtectedMode,
  Efi8259MaxMode
} EFI_8259_MODE;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_SET_VECTOR_BASE) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  UINT8                             MasterBase,
  IN  UINT8                             SlaveBase
  )
/*++

  Routine Description:
    Get the 8259 interrupt masks for Irq0 - Irq15. A different mask exists for
    the legacy mode mask and the protected mode mask. The base address for the 8259
    is different for legacy and protected mode, so two masks are required.

  Arguments:
    This          - Protocol instance pointer.
    MasterBase    - The base vector for the Master PIC in the 8259 controller
    Slavebase     - The base vector for the Master PIC in the 8259 controller

  Returns:
    EFI_SUCCESS       - The new bases were programmed
    EFI_DEVICE_ERROR  - A device erro occured programming the vector bases

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_GET_MASK) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  OUT UINT16                            *LegacyMask, OPTIONAL
  OUT UINT16                            *LegacyEdgeLevel, OPTIONAL
  OUT UINT16                            *ProtectedMask, OPTIONAL
  OUT UINT16                            *ProtectedEdgeLevel OPTIONAL
  )
/*++

  Routine Description:
    Get the 8259 interrupt masks for Irq0 - Irq15. A different mask exists for
    the legacy mode mask and the protected mode mask. The base address for the 8259
    is different for legacy and protected mode, so two masks are required.

  Arguments:
    This               - Protocol instance pointer.
    LegacyMask         - Bit 0 is Irq0 - Bit 15 is Irq15
    LegacyEdgeLevel    - Bit 0 is Irq0 - Bit 15 is Irq15
    ProtectedMask      - Bit 0 is Irq0 - Bit 15 is Irq15
    ProtectedEdgeLevel - Bit 0 is Irq0 - Bit 15 is Irq15

  Returns:
    EFI_SUCCESS       - 8259 status returned
    EFI_DEVICE_ERROR  - Error reading 8259

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_SET_MASK) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  UINT16                            *LegacyMask, OPTIONAL
  IN  UINT16                            *LegacyEdgeLevel, OPTIONAL
  IN  UINT16                            *ProtectedMask, OPTIONAL
  IN  UINT16                            *ProtectedEdgeLevel OPTIONAL
  )
/*++

  Routine Description:
    Set the 8259 interrupt masks for Irq0 - Irq15. A different mask exists for
    the legacy mode mask and the protected mode mask. The base address for the 8259
    is different for legacy and protected mode, so two masks are required.
    Also set the edge/level masks.

  Arguments:
    This               - Protocol instance pointer.
    LegacyMask         - Bit 0 is Irq0 - Bit 15 is Irq15
    LegacyEdgeLevel    - Bit 0 is Irq0 - Bit 15 is Irq15
    ProtectedMask      - Bit 0 is Irq0 - Bit 15 is Irq15
    ProtectedEdgeLevel - Bit 0 is Irq0 - Bit 15 is Irq15

  Returns:
    EFI_SUCCESS       - 8259 status returned
    EFI_DEVICE_ERROR  - Error reading 8259

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_SET_MODE) (
  IN EFI_LEGACY_8259_PROTOCOL         * This,
  IN  EFI_8259_MODE                   Mode,
  IN  UINT16                          *Mask, OPTIONAL
  IN  UINT16                          *EdgeLevel OPTIONAL
  )
/*++

  Routine Description:
    Set the 8259 mode of operation. The base address for the 8259 is different for
    legacy and protected mode. The legacy mode requires the master 8259 to have a
    master base of 0x08 and the slave base of 0x70. The protected mode base locations
    are not defined. Interrupts must be masked by the caller before this function
    is called. The interrupt mask from the current mode is saved. The interrupt 
    mask for the new mode is Mask, or if Mask does not exist the previously saved
    mask is used.


  Arguments:
    This      - Protocol instance pointer.
    Mode      - Mode of operation. i.e. real mode or protected mode
    Mask      - Optional interupt mask for the new mode.
    EdgeLevel - Optional trigger mask for the new mode.

  Returns:
    EFI_SUCCESS       - 8259 programmed
    EFI_DEVICE_ERROR  - Error writting to 8259

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_GET_VECTOR) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  EFI_8259_IRQ                      Irq,
  OUT UINT8                             *Vector
  )
/*++

  Routine Description:
    Convert from IRQ to processor interrupt vector number.

  Arguments:
    This    - Protocol instance pointer.
    Irq     - 8259 IRQ0 - IRQ15
    Vector  - Processor vector number that matches Irq

  Returns:
    EFI_SUCCESS           - The Vector matching Irq is returned
    EFI_INVALID_PARAMETER - Irq not valid

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_ENABLE_IRQ) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  EFI_8259_IRQ                      Irq,
  IN  BOOLEAN                           LevelTriggered
  )
/*++

  Routine Description:
    Enable Irq by unmasking interrupt in 8259

  Arguments:
    This           - Protocol instance pointer.
    Irq            - 8259 IRQ0 - IRQ15
    LevelTriggered - TRUE if level triggered.  FALSE if edge triggered.

  Returns:
    EFI_SUCCESS           - Irq enabled on 8259
    EFI_INVALID_PARAMETER - Irq not valid

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_DISABLE_IRQ) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  EFI_8259_IRQ                      Irq
  )
/*++

  Routine Description:
    Disable Irq by masking interrupt in 8259

  Arguments:
    This    - Protocol instance pointer.
    Irq     - 8259 IRQ0 - IRQ15

  Returns:
    EFI_SUCCESS           - Irq disabled on 8259
    EFI_INVALID_PARAMETER - Irq not valid

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_GET_INTERRUPT_LINE) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  EFI_HANDLE                        PciHandle,
  OUT UINT8                             *Vector
  )
/*++

  Routine Description:
    PciHandle represents a PCI config space of a PCI function. Vector 
    represents Interrupt Pin (from PCI config space) and it is the data
    that is programmed into the Interrupt Line (from the PCI config space)
    register.

  Arguments:
    This      - Protocol instance pointer.
    PciHandle - PCI function to return vector for 
    Vector    - Vector for fucntion that matches 

  Returns:
    EFI_SUCCESS           - A valid Vector is returned
    EFI_INVALID_PARAMETER - PciHandle not valid

--*/
;

typedef
EFI_STATUS
(EFIAPI *EFI_LEGACY_8259_END_OF_INTERRUPT) (
  IN EFI_LEGACY_8259_PROTOCOL           * This,
  IN  EFI_8259_IRQ                      Irq
  )
/*++

  Routine Description:
    Send an EOI to 8259

  Arguments:
    This    - Protocol instance pointer.
    Irq     - 8259 IRQ0 - IRQ15

  Returns:
    EFI_SUCCESS           - EOI successfully sent to 8259
    EFI_INVALID_PARAMETER - Irq not valid

--*/
;

struct _EFI_LEGACY_8259_PROTOCOL {
  EFI_LEGACY_8259_SET_VECTOR_BASE     SetVectorBase;
  EFI_LEGACY_8259_GET_MASK            GetMask;
  EFI_LEGACY_8259_SET_MASK            SetMask;
  EFI_LEGACY_8259_SET_MODE            SetMode;
  EFI_LEGACY_8259_GET_VECTOR          GetVector;
  EFI_LEGACY_8259_ENABLE_IRQ          EnableIrq;
  EFI_LEGACY_8259_DISABLE_IRQ         DisableIrq;
  EFI_LEGACY_8259_GET_INTERRUPT_LINE  GetInterruptLine;
  EFI_LEGACY_8259_END_OF_INTERRUPT    EndOfInterrupt;
};

extern EFI_GUID gEfiLegacy8259ProtocolGuid;

#endif
