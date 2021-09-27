/** @file
This file include all platform action which can be customized by IBV/OEM.

Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformBootManager.h"
#include "PlatformConsole.h"

#define PCI_DEVICE_PATH_NODE(Func, Dev) \
  { \
    { \
      HARDWARE_DEVICE_PATH, \
      HW_PCI_DP, \
      { \
        (UINT8) (sizeof (PCI_DEVICE_PATH)), \
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8) \
      } \
    }, \
    (Func), \
    (Dev) \
  }

#define PNPID_DEVICE_PATH_NODE(PnpId) \
  { \
    { \
      ACPI_DEVICE_PATH, \
      ACPI_DP, \
      { \
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), \
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8) \
      }, \
    }, \
    EISA_PNP_ID((PnpId)), \
    0 \
  }

#define gPciRootBridge \
  PNPID_DEVICE_PATH_NODE(0x0A03)

#define gPnp16550ComPort \
  PNPID_DEVICE_PATH_NODE(0x0501)

#define gUartVendor \
  { \
    { \
      HARDWARE_DEVICE_PATH, \
      HW_VENDOR_DP, \
      { \
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)), \
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8) \
      } \
    }, \
    {0xD3987D4B, 0x971A, 0x435F, {0x8C, 0xAF, 0x49, 0x67, 0xEB, 0x62, 0x72, 0x41}} \
  }

#define gUart \
  { \
    { \
      MESSAGING_DEVICE_PATH, \
      MSG_UART_DP, \
      { \
        (UINT8) (sizeof (UART_DEVICE_PATH)), \
        (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8) \
      } \
    }, \
    0, \
    115200, \
    8, \
    1, \
    1 \
  }

#define gPcAnsiTerminal \
  { \
    { \
      MESSAGING_DEVICE_PATH, \
      MSG_VENDOR_DP, \
      { \
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)), \
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8) \
      } \
    }, \
    DEVICE_PATH_MESSAGING_PC_ANSI \
  }


ACPI_HID_DEVICE_PATH       gPnp16550ComPortDeviceNode = gPnp16550ComPort;
UART_DEVICE_PATH           gUartDeviceNode            = gUart;
VENDOR_DEVICE_PATH         gTerminalTypeDeviceNode    = gPcAnsiTerminal;
VENDOR_DEVICE_PATH         gUartDeviceVendorNode      = gUartVendor;

//
// Predefined platform root bridge
//
PLATFORM_ROOT_BRIDGE_DEVICE_PATH  gPlatformRootBridge0 = {
  gPciRootBridge,
  gEndEntire
};

EFI_DEVICE_PATH_PROTOCOL          *gPlatformRootBridges[] = {
  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformRootBridge0,
  NULL
};

BOOLEAN       mDetectVgaOnly;

/**
  Add UART to ConOut, ConIn, ErrOut.

  @param[in]   DeviceHandle - LPC device path.

  @retval EFI_SUCCESS  - Serial console is added to ConOut, ConIn, and ErrOut.
  @retval EFI_STATUS   - No serial console is added.
**/
EFI_STATUS
PrepareLpcBridgeDevicePath (
  IN EFI_HANDLE                DeviceHandle
)
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  DevicePath = NULL;
  Status = gBS->HandleProtocol (
             DeviceHandle,
             &gEfiDevicePathProtocolGuid,
             (VOID*)&DevicePath
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register COM1
  //
  DevicePath = AppendDevicePathNode ((EFI_DEVICE_PATH_PROTOCOL *)NULL, (EFI_DEVICE_PATH_PROTOCOL *)&gUartDeviceVendorNode);
  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&gUartDeviceNode);
  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&gTerminalTypeDeviceNode);

  EfiBootManagerUpdateConsoleVariable (ConOut, DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConIn, DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, DevicePath, NULL);

  return EFI_SUCCESS;
}

/**
  Return the GOP device path in the platform.

  @param[in]   PciDevicePath - Device path for the PCI graphics device.
  @param[out]  GopDevicePath - Return the device path with GOP installed.

  @retval EFI_SUCCESS  - PCI VGA is added to ConOut.
  @retval EFI_INVALID_PARAMETER   - The device path parameter is invalid.
  @retval EFI_STATUS   - No GOP device found.
**/
EFI_STATUS
GetGopDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL *PciDevicePath,
  OUT EFI_DEVICE_PATH_PROTOCOL **GopDevicePath
)
{
  UINTN                           Index;
  EFI_STATUS                      Status;
  EFI_HANDLE                      PciDeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *TempPciDevicePath;
  UINTN                           GopHandleCount;
  EFI_HANDLE                      *GopHandleBuffer;
  ACPI_ADR_DEVICE_PATH             AcpiAdr;
  EFI_DEVICE_PATH_PROTOCOL        *MyDevicePath;

  if (PciDevicePath == NULL || GopDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MyDevicePath = NULL;

  //
  // Initialize the GopDevicePath to be PciDevicePath
  //
  *GopDevicePath    = PciDevicePath;
  TempPciDevicePath = PciDevicePath;

  Status = gBS->LocateDevicePath (
             &gEfiDevicePathProtocolGuid,
             &TempPciDevicePath,
             &PciDeviceHandle
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Try to connect this handle, so that GOP driver could start on this
  // device and create child handles with GraphicsOutput Protocol installed
  // on them, then we get device paths of these child handles and select
  // them as possible console device.
  //
  AcpiAdr.Header.Type     = ACPI_DEVICE_PATH;
  AcpiAdr.Header.SubType  = ACPI_ADR_DP;
  AcpiAdr.ADR= ACPI_DISPLAY_ADR (1, 0, 0, 1, 0, ACPI_ADR_DISPLAY_TYPE_INTERNAL_DIGITAL, 8, 0);

  SetDevicePathNodeLength (&AcpiAdr.Header, sizeof (ACPI_ADR_DEVICE_PATH));

  MyDevicePath = AppendDevicePathNode(MyDevicePath, (EFI_DEVICE_PATH_PROTOCOL*)&AcpiAdr);

  gBS->ConnectController (PciDeviceHandle, NULL, MyDevicePath, FALSE);

  FreePool(MyDevicePath);

  Status = gBS->LocateHandleBuffer (
             ByProtocol,
             &gEfiGraphicsOutputProtocolGuid,
             NULL,
             &GopHandleCount,
             &GopHandleBuffer
           );
  if (!EFI_ERROR (Status)) {
    //
    // Add all the child handles as possible Console Device
    //
    for (Index = 0; Index < GopHandleCount; Index++) {
      Status = gBS->HandleProtocol (GopHandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID*)&TempDevicePath);
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (CompareMem (
            PciDevicePath,
            TempDevicePath,
            GetDevicePathSize (PciDevicePath) - END_DEVICE_PATH_LENGTH
          ) == 0) {
        //
        // In current implementation, we only enable one of the child handles
        // as console device, i.e. sotre one of the child handle's device
        // path to variable "ConOut"
        // In future, we could select all child handles to be console device
        //
        *GopDevicePath = TempDevicePath;

        //
        // Delete the PCI device's path that added by GetPlugInPciVgaDevicePath()
        // Add the integrity GOP device path.
        //
        EfiBootManagerUpdateConsoleVariable (ConOut, NULL, PciDevicePath);
        EfiBootManagerUpdateConsoleVariable (ConOut, TempDevicePath, NULL);
      }
    }
    gBS->FreePool (GopHandleBuffer);
  }

  return EFI_SUCCESS;
}

/**
  Add PCI VGA to ConOut, ConIn, ErrOut.

  @param[in]  DeviceHandle - Handle of PciIo protocol.

  @retval EFI_SUCCESS  - PCI VGA is added to ConOut.
  @retval EFI_STATUS   - No PCI VGA device is added.

**/
EFI_STATUS
PreparePciVgaDevicePath (
  IN EFI_HANDLE                DeviceHandle
)
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *GopDevicePath;

  DevicePath = NULL;
  Status = gBS->HandleProtocol (
             DeviceHandle,
             &gEfiDevicePathProtocolGuid,
             (VOID*)&DevicePath
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GetGopDevicePath (DevicePath, &GopDevicePath);
  DevicePath = GopDevicePath;

  EfiBootManagerUpdateConsoleVariable (ConOut, DevicePath, NULL);

  return EFI_SUCCESS;
}

/**
  Add PCI Serial to ConOut, ConIn, ErrOut.

  @param[in]  DeviceHandle - Handle of PciIo protocol.

  @retval EFI_SUCCESS  - PCI Serial is added to ConOut, ConIn, and ErrOut.
  @retval EFI_STATUS   - No PCI Serial device is added.

**/
EFI_STATUS
PreparePciSerialDevicePath (
  IN EFI_HANDLE                DeviceHandle
)
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  DevicePath = NULL;
  Status = gBS->HandleProtocol (
             DeviceHandle,
             &gEfiDevicePathProtocolGuid,
             (VOID*)&DevicePath
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&gUartDeviceNode);
  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *)&gTerminalTypeDeviceNode);

  EfiBootManagerUpdateConsoleVariable (ConOut, DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConIn,  DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ErrOut, DevicePath, NULL);

  return EFI_SUCCESS;
}


/**
  For every PCI instance execute a callback function.

  @param[in]  Id                 - The protocol GUID for callback
  @param[in]  CallBackFunction   - The callback function
  @param[in]  Context    - The context of the callback

  @retval EFI_STATUS - Callback function failed.

**/
EFI_STATUS
EFIAPI
VisitAllInstancesOfProtocol (
  IN EFI_GUID                    *Id,
  IN PROTOCOL_INSTANCE_CALLBACK  CallBackFunction,
  IN VOID                        *Context
)
{
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  UINTN                     Index;
  VOID                      *Instance;

  //
  // Start to check all the PciIo to find all possible device
  //
  HandleCount = 0;
  HandleBuffer = NULL;
  Status = gBS->LocateHandleBuffer (
             ByProtocol,
             Id,
             NULL,
             &HandleCount,
             &HandleBuffer
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], Id, &Instance);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = (*CallBackFunction) (
               HandleBuffer[Index],
               Instance,
               Context
             );
  }

  gBS->FreePool (HandleBuffer);

  return EFI_SUCCESS;
}


/**
  For every PCI instance execute a callback function.

  @param[in]  Handle     - The PCI device handle
  @param[in]  Instance   - The instance of the PciIo protocol
  @param[in]  Context    - The context of the callback

  @retval EFI_STATUS - Callback function failed.

**/
EFI_STATUS
EFIAPI
VisitingAPciInstance (
  IN EFI_HANDLE  Handle,
  IN VOID        *Instance,
  IN VOID        *Context
)
{
  EFI_STATUS                Status;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                Pci;

  PciIo = (EFI_PCI_IO_PROTOCOL*) Instance;

  //
  // Check for all PCI device
  //
  Status = PciIo->Pci.Read (
             PciIo,
             EfiPciIoWidthUint32,
             0,
             sizeof (Pci) / sizeof (UINT32),
             &Pci
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return (*(VISIT_PCI_INSTANCE_CALLBACK)(UINTN) Context) (
           Handle,
           PciIo,
           &Pci
         );

}


/**
  For every PCI instance execute a callback function.

  @param[in]  CallBackFunction - Callback function pointer

  @retval EFI_STATUS - Callback function failed.

**/
EFI_STATUS
EFIAPI
VisitAllPciInstances (
  IN VISIT_PCI_INSTANCE_CALLBACK CallBackFunction
)
{
  return VisitAllInstancesOfProtocol (
           &gEfiPciIoProtocolGuid,
           VisitingAPciInstance,
           (VOID*)(UINTN) CallBackFunction
         );
}


/**
  Do platform specific PCI Device check and add them to
  ConOut, ConIn, ErrOut.

  @param[in]  Handle - Handle of PCI device instance
  @param[in]  PciIo - PCI IO protocol instance
  @param[in]  Pci - PCI Header register block

  @retval EFI_SUCCESS - PCI Device check and Console variable update successfully.
  @retval EFI_STATUS - PCI Device check or Console variable update fail.

**/
EFI_STATUS
EFIAPI
DetectAndPreparePlatformPciDevicePath (
  IN EFI_HANDLE           Handle,
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN PCI_TYPE00           *Pci
)
{
  EFI_STATUS                Status;

  Status = PciIo->Attributes (
             PciIo,
             EfiPciIoAttributeOperationEnable,
             EFI_PCI_DEVICE_ENABLE,
             NULL
           );
  ASSERT_EFI_ERROR (Status);

  if (!mDetectVgaOnly) {
    //
    // Here we decide whether it is LPC Bridge
    //
    if ((IS_PCI_LPC (Pci)) ||
        ((IS_PCI_ISA_PDECODE (Pci)) &&
         (Pci->Hdr.VendorId == 0x8086)
        )
       ) {
      //
      // Add IsaKeyboard to ConIn,
      // add IsaSerial to ConOut, ConIn, ErrOut
      //
      DEBUG ((EFI_D_INFO, "Found LPC Bridge device\n"));
      PrepareLpcBridgeDevicePath (Handle);
      return EFI_SUCCESS;
    }
    //
    // Here we decide which Serial device to enable in PCI bus
    //
    if (IS_PCI_16550SERIAL (Pci)) {
      //
      // Add them to ConOut, ConIn, ErrOut.
      //
      DEBUG ((EFI_D_INFO, "Found PCI 16550 SERIAL device\n"));
      PreparePciSerialDevicePath (Handle);
      return EFI_SUCCESS;
    }
  }

  //
  // Here we decide which VGA device to enable in PCI bus
  //
  if (IS_PCI_VGA (Pci)) {
    //
    // Add them to ConOut.
    //
    DEBUG ((EFI_D_INFO, "Found PCI VGA device\n"));
    PreparePciVgaDevicePath (Handle);
    return EFI_SUCCESS;
  }

  return Status;
}


/**
  Do platform specific PCI Device check and add them to ConOut, ConIn, ErrOut

  @param[in]  DetectVgaOnly - Only detect VGA device if it's TRUE.

  @retval EFI_SUCCESS - PCI Device check and Console variable update successfully.
  @retval EFI_STATUS - PCI Device check or Console variable update fail.

**/
EFI_STATUS
DetectAndPreparePlatformPciDevicePaths (
  BOOLEAN DetectVgaOnly
)
{
  mDetectVgaOnly = DetectVgaOnly;
  return VisitAllPciInstances (DetectAndPreparePlatformPciDevicePath);
}


/**
  The function will connect root bridge

   @return EFI_SUCCESS      Connect RootBridge successfully.

**/
EFI_STATUS
ConnectRootBridge (
  VOID
)
{
  EFI_STATUS                Status;
  EFI_HANDLE                RootHandle;

  //
  // Make all the PCI_IO protocols on PCI Seg 0 show up
  //
  Status = gBS->LocateDevicePath (
             &gEfiDevicePathProtocolGuid,
             &gPlatformRootBridges[0],
             &RootHandle
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->ConnectController (RootHandle, NULL, NULL, FALSE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Platform console init. Include the platform firmware vendor, revision
  and so crc check.

**/
VOID
EFIAPI
PlatformConsoleInit (
  VOID
)
{
  gUartDeviceNode.BaudRate = PcdGet64 (PcdUartDefaultBaudRate);
  gUartDeviceNode.DataBits = PcdGet8 (PcdUartDefaultDataBits);
  gUartDeviceNode.Parity   = PcdGet8 (PcdUartDefaultParity);
  gUartDeviceNode.StopBits = PcdGet8 (PcdUartDefaultStopBits);

  ConnectRootBridge ();

  //
  // Do platform specific PCI Device check and add them to ConOut, ConIn, ErrOut
  //
  DetectAndPreparePlatformPciDevicePaths (FALSE);
}
