/*++

Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.           


Module Name:

  RuntimeLib.c

Abstract:

  Runtime lib

--*/

#include "../RuntimeLibInternal.h"

//
// Driver Lib Module Globals
//

STATIC EFI_EVENT              mEfiVirtualNotifyEvent;
STATIC BOOLEAN                mEfiGoneVirtual         = FALSE;
STATIC BOOLEAN                mEfiAtRuntime           = FALSE;
EFI_RUNTIME_SERVICES          *mRTEdkDxeRuntimeDriverLib;

/**
  Set AtRuntime flag as TRUE after ExitBootServices
  
  @param[in]  Event   The Event that is being processed
  @param[in]  Context Event Context
**/
VOID
EFIAPI
RuntimeDriverExitBootServices (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  //
  // Clear out BootService globals
  //
  gBS             = NULL;

  mEfiAtRuntime = TRUE;
}

/**
  Fixup internal data so that EFI can be call in virtual mode.
  Call the passed in Child Notify event and convert any pointers in 
  lib to virtual mode.
  
  @param[in]    Event   The Event that is being processed
  @param[in]    Context Event Context
**/
STATIC
VOID
EFIAPI
RuntimeLibVirtualNotifyEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  UINTN Index;
  EFI_EVENT_NOTIFY  ChildNotifyEventHandler;

  for (Index = 0; 
       _gDriverSetVirtualAddressMapEvent[Index] != NULL;
       Index++) {
    ChildNotifyEventHandler = _gDriverSetVirtualAddressMapEvent[Index];
    ChildNotifyEventHandler (Event, NULL);
  }

  //
  // Update global for Runtime Services Table and IO
  //
  EfiConvertPointer (0, (VOID **) &mRTEdkDxeRuntimeDriverLib);

  mEfiGoneVirtual = TRUE;
}

/**
  Intialize runtime Driver Lib if it has not yet been initialized. 
  
  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.
  
  @return     EFI_STATUS    always returns EFI_SUCCESS except EFI_ALREADY_STARTED if already started.
**/
EFI_STATUS
EFIAPI
RuntimeDriverLibConstruct (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;

  ASSERT (SystemTable != NULL);
  mRTEdkDxeRuntimeDriverLib = SystemTable->RuntimeServices;
  ASSERT (mRTEdkDxeRuntimeDriverLib != NULL);
  
  //
  // Register SetVirtualAddressMap () notify function
  // 
  ASSERT (gBS != NULL);
  Status = gBS->CreateEvent (
                  EFI_EVENT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
                  EFI_TPL_NOTIFY,
                  RuntimeLibVirtualNotifyEvent,
                  NULL,
                  &mEfiVirtualNotifyEvent
                  );

  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  This routine will free some resources which have been allocated in
  EfiInitializeRuntimeDriverLib(). If a runtime driver exits with an error, 
  it must call this routine to free the allocated resource before the exiting.

  @retval     EFI_SUCCESS       Shutdown the Runtime Driver Lib successfully
  @retval     EFI_UNSUPPORTED   Runtime Driver lib was not initialized at all
**/
EFI_STATUS
EFIAPI
RuntimeDriverLibDeconstruct (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  //
  // Close SetVirtualAddressMap () notify function
  //
  ASSERT (gBS != NULL);
  Status = gBS->CloseEvent (mEfiVirtualNotifyEvent);
  ASSERT_EFI_ERROR (Status);
  
  return Status;
}

/**
  Return TRUE if ExitBootServices () has been called
  
  @retval TRUE If ExitBootServices () has been called
**/
BOOLEAN
EFIAPI
EfiAtRuntime (
  VOID
  )
{
  return mEfiAtRuntime;
}

/**
  Return TRUE if SetVirtualAddressMap () has been called
  
  @retval TRUE  If SetVirtualAddressMap () has been called
**/
BOOLEAN
EFIAPI
EfiGoneVirtual (
  VOID
  )
{
  return mEfiGoneVirtual;
}

