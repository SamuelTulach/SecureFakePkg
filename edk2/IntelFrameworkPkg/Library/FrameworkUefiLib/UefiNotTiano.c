/** @file
  Library functions that abstract areas of conflict between framework and UEFI 2.0.

  Help Port Framework code that has conflicts with UEFI 2.0 by hiding the
  old conflicts with library functions and supporting implementations of the old
  (EDK/EFI 1.10) and new (EDK II/UEFI 2.0) way. This module is a DXE driver as
  it contains DXE enum extensions for EFI event services.

Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/



#include "UefiLibInternal.h"

/**
  Create a Legacy Boot Event.

  Tiano extended the CreateEvent Type enum to add a legacy boot event type.
  This was bad as Tiano did not own the enum. In UEFI 2.0 CreateEventEx was
  added and now it's possible to not voilate the UEFI specification by
  declaring a GUID for the legacy boot event class. This library supports
  the EDK/EFI 1.10 form and EDK II/UEFI 2.0 form and allows common code to
  work both ways.

  @param  LegacyBootEvent   Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
EfiCreateEventLegacyBoot (
  OUT EFI_EVENT  *LegacyBootEvent
  )
{
  return EfiCreateEventLegacyBootEx (
           TPL_CALLBACK,
           EfiEventEmptyFunction,
           NULL,
           LegacyBootEvent
           );
}

/**
  Create an EFI event in the Legacy Boot Event Group and allows
  the caller to specify a notification function.

  This function abstracts the creation of the Legacy Boot Event.
  The Framework moved from a proprietary to UEFI 2.0 based mechanism.
  This library abstracts the caller from how this event is created to prevent
  to code form having to change with the version of the specification supported.
  If LegacyBootEvent is NULL, then ASSERT().

  @param  NotifyTpl         The task priority level of the event.
  @param  NotifyFunction    The notification function to call when the event is signaled.
  @param  NotifyContext     The content to pass to NotifyFunction when the event is signaled.
  @param  LegacyBootEvent   Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
EfiCreateEventLegacyBootEx (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,  OPTIONAL
  IN  VOID              *NotifyContext,  OPTIONAL
  OUT EFI_EVENT         *LegacyBootEvent
  )
{
  EFI_STATUS    Status;

  ASSERT (LegacyBootEvent != NULL);

  if (gST->Hdr.Revision < 0x00020000) {
    //
    // prior to UEFI 2.0 use Tiano extension to EFI
    //
    Status = gBS->CreateEvent (
                    EFI_EVENT_SIGNAL_LEGACY_BOOT | EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    LegacyBootEvent
                    );
  } else {
    //
    // For UEFI 2.0 and the future use an Event Group
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    &gEfiEventLegacyBootGuid,
                    LegacyBootEvent
                    );
  }

  return Status;
}

/**
  Create a Read to Boot Event.

  Tiano extended the CreateEvent Type enum to add a ready to boot event type.
  This was bad as Tiano did not own the enum. In UEFI 2.0 CreateEventEx was
  added and now it's possible to not voilate the UEFI specification and use
  the ready to boot event class defined in UEFI 2.0. This library supports
  the EDK/EFI 1.10 form and EDK II/UEFI 2.0 form and allows common code to
  work both ways.

  @param  ReadyToBootEvent   Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
EfiCreateEventReadyToBoot (
  OUT EFI_EVENT  *ReadyToBootEvent
  )
{
  return EfiCreateEventReadyToBootEx (
           TPL_CALLBACK,
           EfiEventEmptyFunction,
           NULL,
           ReadyToBootEvent
           );
}

/**
  Create an EFI event in the Ready To Boot Event Group and allows
  the caller to specify a notification function.

  This function abstracts the creation of the Ready to Boot Event.
  The Framework moved from a proprietary to UEFI 2.0 based mechanism.
  This library abstracts the caller from how this event is created to prevent
  to code form having to change with the version of the specification supported.
  If ReadyToBootEvent is NULL, then ASSERT().

  @param  NotifyTpl         The task priority level of the event.
  @param  NotifyFunction    The notification function to call when the event is signaled.
  @param  NotifyContext     The content to pass to NotifyFunction when the event is signaled.
  @param  ReadyToBootEvent  Returns the EFI event returned from gBS->CreateEvent(Ex).

  @retval EFI_SUCCESS       Event was created.
  @retval Other             Event was not created.

**/
EFI_STATUS
EFIAPI
EfiCreateEventReadyToBootEx (
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction,  OPTIONAL
  IN  VOID              *NotifyContext,  OPTIONAL
  OUT EFI_EVENT         *ReadyToBootEvent
  )
{
  EFI_STATUS    Status;

  ASSERT (ReadyToBootEvent != NULL);

  if (gST->Hdr.Revision < 0x00020000) {
    //
    // prior to UEFI 2.0 use Tiano extension to EFI
    //
    Status = gBS->CreateEvent (
                    EFI_EVENT_SIGNAL_READY_TO_BOOT | EFI_EVENT_NOTIFY_SIGNAL_ALL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    ReadyToBootEvent
                    );
  } else {
    //
    // For UEFI 2.0 and the future use an Event Group
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    NotifyTpl,
                    NotifyFunction,
                    NotifyContext,
                    &gEfiEventReadyToBootGuid,
                    ReadyToBootEvent
                    );
  }

  return Status;
}


/**
  Signal a Ready to Boot Event.

  Create a Ready to Boot Event. Signal it and close it. This causes other
  events of the same event group to be signaled in other modules.

**/
VOID
EFIAPI
EfiSignalEventReadyToBoot (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     ReadyToBootEvent;

  Status = EfiCreateEventReadyToBoot (&ReadyToBootEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (ReadyToBootEvent);
    gBS->CloseEvent (ReadyToBootEvent);
  }
}

/**
  Signal a Legacy Boot Event.

  Create a legacy Boot Event. Signal it and close it. This causes other
  events of the same event group to be signaled in other modules.

**/
VOID
EFIAPI
EfiSignalEventLegacyBoot (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     LegacyBootEvent;

  Status = EfiCreateEventLegacyBoot (&LegacyBootEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (LegacyBootEvent);
    gBS->CloseEvent (LegacyBootEvent);
  }
}


/**
  Check to see if the Firmware Volume (FV) Media Device Path is valid

  Tiano extended the EFI 1.10 device path nodes. Tiano does not own this enum
  so as we move to UEFI 2.0 support we must use a mechanism that conforms with
  the UEFI 2.0 specification to define the FV device path. An UEFI GUIDed
  device path is defined for Tiano extensions of device path. If the code
  is compiled to conform with the UEFI 2.0 specification use the new device path
  else use the old form for backwards compatability. The return value to this
  function points to a location in FvDevicePathNode and it does not allocate
  new memory for the GUID pointer that is returned.

  @param  FvDevicePathNode  Pointer to FV device path to check.

  @retval NULL              FvDevicePathNode is not valid.
  @retval Other             FvDevicePathNode is valid and pointer to NameGuid was returned.

**/
EFI_GUID *
EFIAPI
EfiGetNameGuidFromFwVolDevicePathNode (
  IN CONST MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  *FvDevicePathNode
  )
{
  ASSERT (FvDevicePathNode != NULL);

  //
  // EFI Specification extension on Media Device Path. MEDIA_FW_VOL_FILEPATH_DEVICE_PATH is adopted by UEFI later and added in UEFI2.10.
  // In EdkCompatibility Package, we only support MEDIA_FW_VOL_FILEPATH_DEVICE_PATH that complies with
  // EFI 1.10 and UEFI 2.10.
  //
  if (DevicePathType (&FvDevicePathNode->Header) == MEDIA_DEVICE_PATH &&
      DevicePathSubType (&FvDevicePathNode->Header) == MEDIA_PIWG_FW_FILE_DP) {
    return (EFI_GUID *) &FvDevicePathNode->FvFileName;
  }

  return NULL;
}


/**
  Initialize a Firmware Volume (FV) Media Device Path node.

  Tiano extended the EFI 1.10 device path nodes. Tiano does not own this enum
  so as we move to UEFI 2.0 support we must use a mechanism that conforms with
  the UEFI 2.0 specification to define the FV device path. An UEFI GUIDed
  device path is defined for Tiano extensions of device path. If the code
  is compiled to conform with the UEFI 2.0 specification use the new device path
  else use the old form for backwards compatability.

  @param  FvDevicePathNode  Pointer to a FV device path node to initialize
  @param  NameGuid          FV file name to use in FvDevicePathNode

**/
VOID
EFIAPI
EfiInitializeFwVolDevicepathNode (
  IN OUT MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  *FvDevicePathNode,
  IN CONST EFI_GUID                         *NameGuid
  )
{
  ASSERT (FvDevicePathNode  != NULL);
  ASSERT (NameGuid          != NULL);

  //
  // EFI Specification extension on Media Device Path. MEDIA_FW_VOL_FILEPATH_DEVICE_PATH is adopted by UEFI later and added in UEFI2.10.
  // In EdkCompatibility Package, we only support MEDIA_FW_VOL_FILEPATH_DEVICE_PATH that complies with
  // EFI 1.10 and UEFI 2.10.
  //
  FvDevicePathNode->Header.Type     = MEDIA_DEVICE_PATH;
  FvDevicePathNode->Header.SubType  = MEDIA_PIWG_FW_FILE_DP;
  SetDevicePathNodeLength (&FvDevicePathNode->Header, sizeof (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH));

  CopyGuid (&FvDevicePathNode->FvFileName, NameGuid);
}

