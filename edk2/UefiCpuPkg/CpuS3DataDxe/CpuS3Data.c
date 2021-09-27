/** @file
ACPI CPU Data initialization module

This module initializes the ACPI_CPU_DATA structure and registers the address
of this structure in the PcdCpuS3DataAddress PCD.  This is a generic/simple
version of this module.  It does not provide a machine check handler or CPU
register initialization tables for ACPI S3 resume.  It also only supports the
number of CPUs reported by the MP Services Protocol, so this module does not
support hot plug CPUs.  This module can be copied into a CPU specific package
and customized if these additional features are required.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2015, Red Hat, Inc.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>

#include <AcpiCpuData.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MtrrLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/MpService.h>
#include <Guid/EventGroup.h>

//
// Data structure used to allocate ACPI_CPU_DATA and its supporting structures
//
typedef struct {
  ACPI_CPU_DATA             AcpiCpuData;
  MTRR_SETTINGS             MtrrTable;
  IA32_DESCRIPTOR           GdtrProfile;
  IA32_DESCRIPTOR           IdtrProfile;
} ACPI_CPU_DATA_EX;

/**
  Allocate EfiACPIMemoryNVS memory.

  @param[in] Size   Size of memory to allocate.

  @return       Allocated address for output.

**/
VOID *
AllocateAcpiNvsMemory (
  IN UINTN  Size
  )
{
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;
  VOID                  *Buffer;

  Status  = gBS->AllocatePages (
                   AllocateAnyPages,
                   EfiACPIMemoryNVS,
                   EFI_SIZE_TO_PAGES (Size),
                   &Address
                   );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Buffer = (VOID *)(UINTN)Address;
  ZeroMem (Buffer, Size);

  return Buffer;
}

/**
  Allocate memory and clean it with zero.

  @param[in] Size   Size of memory to allocate.

  @return       Allocated address for output.

**/
VOID *
AllocateZeroPages (
  IN UINTN  Size
  )
{
  VOID                  *Buffer;

  Buffer = AllocatePages (EFI_SIZE_TO_PAGES (Size));
  if (Buffer != NULL) {
    ZeroMem (Buffer, Size);
  }

  return Buffer;
}
/**
  Callback function executed when the EndOfDxe event group is signaled.

  We delay allocating StartupVector and saving the MTRR settings until BDS signals EndOfDxe.

  @param[in]  Event    Event whose notification function is being invoked.
  @param[out] Context  Pointer to the MTRR_SETTINGS buffer to fill in.
**/
VOID
EFIAPI
CpuS3DataOnEndOfDxe (
  IN  EFI_EVENT  Event,
  OUT VOID       *Context
  )
{
  EFI_STATUS         Status;
  ACPI_CPU_DATA_EX   *AcpiCpuDataEx;

  AcpiCpuDataEx = (ACPI_CPU_DATA_EX *) Context;
  //
  // Allocate a 4KB reserved page below 1MB
  //
  AcpiCpuDataEx->AcpiCpuData.StartupVector = BASE_1MB - 1;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  1,
                  &AcpiCpuDataEx->AcpiCpuData.StartupVector
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_VERBOSE, "%a\n", __FUNCTION__));
  MtrrGetAllMtrrs (&AcpiCpuDataEx->MtrrTable);

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);
}

/**
   The entry function of the CpuS3Data driver.

   Allocate and initialize all fields of the ACPI_CPU_DATA structure except the
   MTRR settings.  Register an event notification on gEfiEndOfDxeEventGroupGuid
   to capture the ACPI_CPU_DATA MTRR settings.  The PcdCpuS3DataAddress is set
   to the address that ACPI_CPU_DATA is allocated at.

   @param[in] ImageHandle  The firmware allocated handle for the EFI image.
   @param[in] SystemTable  A pointer to the EFI System Table.

   @retval EFI_SUCCESS     The entry point is executed successfully.
   @retval EFI_UNSUPPORTED Do not support ACPI S3.
   @retval other           Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CpuS3DataInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                 Status;
  ACPI_CPU_DATA_EX           *AcpiCpuDataEx;
  ACPI_CPU_DATA              *AcpiCpuData;
  EFI_MP_SERVICES_PROTOCOL   *MpServices;
  UINTN                      NumberOfCpus;
  UINTN                      NumberOfEnabledProcessors;
  VOID                       *Stack;
  UINTN                      TableSize;
  CPU_REGISTER_TABLE         *RegisterTable;
  UINTN                      Index;
  EFI_PROCESSOR_INFORMATION  ProcessorInfoBuffer;
  UINTN                      GdtSize;
  UINTN                      IdtSize;
  VOID                       *Gdt;
  VOID                       *Idt;
  EFI_EVENT                  Event;
  ACPI_CPU_DATA              *OldAcpiCpuData;

  if (!PcdGetBool (PcdAcpiS3Enable)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Set PcdCpuS3DataAddress to the base address of the ACPI_CPU_DATA structure
  //
  OldAcpiCpuData = (ACPI_CPU_DATA *) (UINTN) PcdGet64 (PcdCpuS3DataAddress);

  AcpiCpuDataEx = AllocateZeroPages (sizeof (ACPI_CPU_DATA_EX));
  ASSERT (AcpiCpuDataEx != NULL);
  AcpiCpuData = &AcpiCpuDataEx->AcpiCpuData;

  //
  // Get MP Services Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **)&MpServices
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Get the number of CPUs
  //
  Status = MpServices->GetNumberOfProcessors (
                         MpServices,
                         &NumberOfCpus,
                         &NumberOfEnabledProcessors
                         );
  ASSERT_EFI_ERROR (Status);
  AcpiCpuData->NumberOfCpus = (UINT32)NumberOfCpus;

  //
  // Initialize ACPI_CPU_DATA fields
  //
  AcpiCpuData->StackSize                 = PcdGet32 (PcdCpuApStackSize);
  AcpiCpuData->ApMachineCheckHandlerBase = 0;
  AcpiCpuData->ApMachineCheckHandlerSize = 0;
  AcpiCpuData->GdtrProfile  = (EFI_PHYSICAL_ADDRESS)(UINTN)&AcpiCpuDataEx->GdtrProfile;
  AcpiCpuData->IdtrProfile  = (EFI_PHYSICAL_ADDRESS)(UINTN)&AcpiCpuDataEx->IdtrProfile;
  AcpiCpuData->MtrrTable    = (EFI_PHYSICAL_ADDRESS)(UINTN)&AcpiCpuDataEx->MtrrTable;

  //
  // Allocate stack space for all CPUs.
  // Use ACPI NVS memory type because this data will be directly used by APs
  // in S3 resume phase in long mode. Also during S3 resume, the stack buffer
  // will only be used as scratch space. i.e. we won't read anything from it
  // before we write to it, in PiSmmCpuDxeSmm.
  //
  Stack = AllocateAcpiNvsMemory (NumberOfCpus * AcpiCpuData->StackSize);
  ASSERT (Stack != NULL);
  AcpiCpuData->StackAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)Stack;

  //
  // Get the boot processor's GDT and IDT
  //
  AsmReadGdtr (&AcpiCpuDataEx->GdtrProfile);
  AsmReadIdtr (&AcpiCpuDataEx->IdtrProfile);

  //
  // Allocate GDT and IDT and copy current GDT and IDT contents
  //
  GdtSize = AcpiCpuDataEx->GdtrProfile.Limit + 1;
  IdtSize = AcpiCpuDataEx->IdtrProfile.Limit + 1;
  Gdt = AllocateZeroPages (GdtSize + IdtSize);
  ASSERT (Gdt != NULL);
  Idt = (VOID *)((UINTN)Gdt + GdtSize);
  CopyMem (Gdt, (VOID *)AcpiCpuDataEx->GdtrProfile.Base, GdtSize);
  CopyMem (Idt, (VOID *)AcpiCpuDataEx->IdtrProfile.Base, IdtSize);
  AcpiCpuDataEx->GdtrProfile.Base = (UINTN)Gdt;
  AcpiCpuDataEx->IdtrProfile.Base = (UINTN)Idt;

  if (OldAcpiCpuData != NULL) {
    AcpiCpuData->RegisterTable           = OldAcpiCpuData->RegisterTable;
    AcpiCpuData->PreSmmInitRegisterTable = OldAcpiCpuData->PreSmmInitRegisterTable;
    AcpiCpuData->ApLocation              = OldAcpiCpuData->ApLocation;
    CopyMem (&AcpiCpuData->CpuStatus, &OldAcpiCpuData->CpuStatus, sizeof (CPU_STATUS_INFORMATION));
  } else {
    //
    // Allocate buffer for empty RegisterTable and PreSmmInitRegisterTable for all CPUs
    //
    TableSize = 2 * NumberOfCpus * sizeof (CPU_REGISTER_TABLE);
    RegisterTable = (CPU_REGISTER_TABLE *)AllocateZeroPages (TableSize);
    ASSERT (RegisterTable != NULL);

    for (Index = 0; Index < NumberOfCpus; Index++) {
      Status = MpServices->GetProcessorInfo (
                           MpServices,
                           Index,
                           &ProcessorInfoBuffer
                           );
      ASSERT_EFI_ERROR (Status);

      RegisterTable[Index].InitialApicId      = (UINT32)ProcessorInfoBuffer.ProcessorId;
      RegisterTable[Index].TableLength        = 0;
      RegisterTable[Index].AllocatedSize      = 0;
      RegisterTable[Index].RegisterTableEntry = 0;

      RegisterTable[NumberOfCpus + Index].InitialApicId      = (UINT32)ProcessorInfoBuffer.ProcessorId;
      RegisterTable[NumberOfCpus + Index].TableLength        = 0;
      RegisterTable[NumberOfCpus + Index].AllocatedSize      = 0;
      RegisterTable[NumberOfCpus + Index].RegisterTableEntry = 0;
    }
    AcpiCpuData->RegisterTable           = (EFI_PHYSICAL_ADDRESS)(UINTN)RegisterTable;
    AcpiCpuData->PreSmmInitRegisterTable = (EFI_PHYSICAL_ADDRESS)(UINTN)(RegisterTable + NumberOfCpus);
  }

  //
  // Set PcdCpuS3DataAddress to the base address of the ACPI_CPU_DATA structure
  //
  Status = PcdSet64S (PcdCpuS3DataAddress, (UINT64)(UINTN)AcpiCpuData);
  ASSERT_EFI_ERROR (Status);

  //
  // Register EFI_END_OF_DXE_EVENT_GROUP_GUID event.
  // The notification function allocates StartupVector and saves MTRRs for ACPI_CPU_DATA
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  CpuS3DataOnEndOfDxe,
                  AcpiCpuData,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
