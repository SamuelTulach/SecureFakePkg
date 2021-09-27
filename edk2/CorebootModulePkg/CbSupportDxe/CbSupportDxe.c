/** @file
  This driver will report some MMIO/IO resources to dxe core, extract smbios and acpi
  tables from coreboot and install.

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include "CbSupportDxe.h"

/**
  Reserve MMIO/IO resource in GCD

  @param  IsMMIO        Flag of whether it is mmio resource or io resource.
  @param  GcdType       Type of the space.
  @param  BaseAddress   Base address of the space.
  @param  Length        Length of the space.
  @param  Alignment     Align with 2^Alignment
  @param  ImageHandle   Handle for the image of this driver.

  @retval EFI_SUCCESS   Reserve successful
**/
EFI_STATUS
CbReserveResourceInGcd (
  IN BOOLEAN               IsMMIO,
  IN UINTN                 GcdType,
  IN EFI_PHYSICAL_ADDRESS  BaseAddress,
  IN UINT64                Length,
  IN UINTN                 Alignment,
  IN EFI_HANDLE            ImageHandle
  )
{
  EFI_STATUS               Status;

  if (IsMMIO) {
    Status = gDS->AddMemorySpace (
                    GcdType,
                    BaseAddress,
                    Length,
                    EFI_MEMORY_UC
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "Failed to add memory space :0x%lx 0x%lx\n",
        BaseAddress,
        Length
        ));
    }
    ASSERT_EFI_ERROR (Status);
    Status = gDS->AllocateMemorySpace (
                    EfiGcdAllocateAddress,
                    GcdType,
                    Alignment,
                    Length,
                    &BaseAddress,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    Status = gDS->AddIoSpace (
                    GcdType,
                    BaseAddress,
                    Length
                    );
    ASSERT_EFI_ERROR (Status);
    Status = gDS->AllocateIoSpace (
                    EfiGcdAllocateAddress,
                    GcdType,
                    Alignment,
                    Length,
                    &BaseAddress,
                    ImageHandle,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}


/**
  Main entry for the Coreboot Support DXE module.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CbDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HOB_GUID_TYPE  *GuidHob;
  SYSTEM_TABLE_INFO  *pSystemTableInfo;
  FRAME_BUFFER_INFO  *FbInfo;

  Status = EFI_SUCCESS;
  //
  // Report MMIO/IO Resources
  //
  Status = CbReserveResourceInGcd (TRUE, EfiGcdMemoryTypeMemoryMappedIo, 0xFEC00000, SIZE_4KB, 0, SystemTable); // IOAPIC
  ASSERT_EFI_ERROR (Status);

  Status = CbReserveResourceInGcd (TRUE, EfiGcdMemoryTypeMemoryMappedIo, 0xFED00000, SIZE_1KB, 0, SystemTable); // HPET
  ASSERT_EFI_ERROR (Status);

  //
  // Find the system table information guid hob
  //
  GuidHob = GetFirstGuidHob (&gUefiSystemTableInfoGuid);
  ASSERT (GuidHob != NULL);
  pSystemTableInfo = (SYSTEM_TABLE_INFO *)GET_GUID_HOB_DATA (GuidHob);

  //
  // Install Acpi Table
  //
  if (pSystemTableInfo->AcpiTableBase != 0 && pSystemTableInfo->AcpiTableSize != 0) {
    DEBUG ((EFI_D_ERROR, "Install Acpi Table at 0x%lx, length 0x%x\n", pSystemTableInfo->AcpiTableBase, pSystemTableInfo->AcpiTableSize));
    Status = gBS->InstallConfigurationTable (&gEfiAcpiTableGuid, (VOID *)(UINTN)pSystemTableInfo->AcpiTableBase);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Install Smbios Table
  //
  if (pSystemTableInfo->SmbiosTableBase != 0 && pSystemTableInfo->SmbiosTableSize != 0) {
    DEBUG ((EFI_D_ERROR, "Install Smbios Table at 0x%lx, length 0x%x\n", pSystemTableInfo->SmbiosTableBase, pSystemTableInfo->SmbiosTableSize));
    Status = gBS->InstallConfigurationTable (&gEfiSmbiosTableGuid, (VOID *)(UINTN)pSystemTableInfo->SmbiosTableBase);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Find the frame buffer information and update PCDs
  //
  GuidHob = GetFirstGuidHob (&gUefiFrameBufferInfoGuid);
  if (GuidHob != NULL) {
    FbInfo  = (FRAME_BUFFER_INFO *)GET_GUID_HOB_DATA (GuidHob);
    Status = PcdSet32S (PcdVideoHorizontalResolution, FbInfo->HorizontalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdVideoVerticalResolution, FbInfo->VerticalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdSetupVideoHorizontalResolution, FbInfo->HorizontalResolution);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdSetupVideoVerticalResolution, FbInfo->VerticalResolution);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

