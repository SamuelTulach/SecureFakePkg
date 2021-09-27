/** @file
  MP initialize support functions for DXE phase.

  Copyright (c) 2016 - 2018, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "MpLib.h"

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugAgentLib.h>
#include <Library/DxeServicesTableLib.h>

#include <Protocol/Timer.h>

#define  AP_CHECK_INTERVAL     (EFI_TIMER_PERIOD_MILLISECONDS (100))
#define  AP_SAFE_STACK_SIZE    128

CPU_MP_DATA      *mCpuMpData = NULL;
EFI_EVENT        mCheckAllApsEvent = NULL;
EFI_EVENT        mMpInitExitBootServicesEvent = NULL;
EFI_EVENT        mLegacyBootEvent = NULL;
volatile BOOLEAN mStopCheckAllApsStatus = TRUE;
VOID             *mReservedApLoopFunc = NULL;
UINTN            mReservedTopOfApStack;
volatile UINT32  mNumberToFinish = 0;

/**
  Enable Debug Agent to support source debugging on AP function.

**/
VOID
EnableDebugAgent (
  VOID
  )
{
  //
  // Initialize Debug Agent to support source level debug in DXE phase
  //
  InitializeDebugAgent (DEBUG_AGENT_INIT_DXE_AP, NULL, NULL);
}

/**
  Get the pointer to CPU MP Data structure.

  @return  The pointer to CPU MP Data structure.
**/
CPU_MP_DATA *
GetCpuMpData (
  VOID
  )
{
  ASSERT (mCpuMpData != NULL);
  return mCpuMpData;
}

/**
  Save the pointer to CPU MP Data structure.

  @param[in] CpuMpData  The pointer to CPU MP Data structure will be saved.
**/
VOID
SaveCpuMpData (
  IN CPU_MP_DATA   *CpuMpData
  )
{
  mCpuMpData = CpuMpData;
}

/**
  Get available system memory below 1MB by specified size.

  @param[in] WakeupBufferSize   Wakeup buffer size required

  @retval other   Return wakeup buffer address below 1MB.
  @retval -1      Cannot find free memory below 1MB.
**/
UINTN
GetWakeupBuffer (
  IN UINTN                WakeupBufferSize
  )
{
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    StartAddress;

  StartAddress = BASE_1MB;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES (WakeupBufferSize),
                  &StartAddress
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = gBS->FreePages(
               StartAddress,
               EFI_SIZE_TO_PAGES (WakeupBufferSize)
               );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "WakeupBufferStart = %x, WakeupBufferSize = %x\n",
                        (UINTN) StartAddress, WakeupBufferSize));
  } else {
    StartAddress = (EFI_PHYSICAL_ADDRESS) -1;
  }
  return (UINTN) StartAddress;
}

/**
  Get available EfiBootServicesCode memory below 4GB by specified size.

  This buffer is required to safely transfer AP from real address mode to
  protected mode or long mode, due to the fact that the buffer returned by
  GetWakeupBuffer() may be marked as non-executable.

  @param[in] BufferSize   Wakeup transition buffer size.

  @retval other   Return wakeup transition buffer address below 4GB.
  @retval 0       Cannot find free memory below 4GB.
**/
UINTN
GetModeTransitionBuffer (
  IN UINTN                BufferSize
  )
{
  EFI_STATUS              Status;
  EFI_PHYSICAL_ADDRESS    StartAddress;

  StartAddress = BASE_4GB - 1;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiBootServicesCode,
                  EFI_SIZE_TO_PAGES (BufferSize),
                  &StartAddress
                  );
  if (EFI_ERROR (Status)) {
    StartAddress = 0;
  }

  return (UINTN)StartAddress;
}

/**
  Checks APs status and updates APs status if needed.

**/
VOID
CheckAndUpdateApsStatus (
  VOID
  )
{
  UINTN                   ProcessorNumber;
  EFI_STATUS              Status;
  CPU_MP_DATA             *CpuMpData;

  CpuMpData = GetCpuMpData ();

  //
  // First, check whether pending StartupAllAPs() exists.
  //
  if (CpuMpData->WaitEvent != NULL) {

    Status = CheckAllAPs ();
    //
    // If all APs finish for StartupAllAPs(), signal the WaitEvent for it.
    //
    if (Status != EFI_NOT_READY) {
      Status = gBS->SignalEvent (CpuMpData->WaitEvent);
      CpuMpData->WaitEvent = NULL;
    }
  }

  //
  // Second, check whether pending StartupThisAPs() callings exist.
  //
  for (ProcessorNumber = 0; ProcessorNumber < CpuMpData->CpuCount; ProcessorNumber++) {

    if (CpuMpData->CpuData[ProcessorNumber].WaitEvent == NULL) {
      continue;
    }

    Status = CheckThisAP (ProcessorNumber);

    if (Status != EFI_NOT_READY) {
      gBS->SignalEvent (CpuMpData->CpuData[ProcessorNumber].WaitEvent);
     CpuMpData->CpuData[ProcessorNumber].WaitEvent = NULL;
    }
  }
}

/**
  Checks APs' status periodically.

  This function is triggered by timer periodically to check the
  state of APs for StartupAllAPs() and StartupThisAP() executed
  in non-blocking mode.

  @param[in]  Event    Event triggered.
  @param[in]  Context  Parameter passed with the event.

**/
VOID
EFIAPI
CheckApsStatus (
  IN  EFI_EVENT                           Event,
  IN  VOID                                *Context
  )
{
  //
  // If CheckApsStatus() is not stopped, otherwise return immediately.
  //
  if (!mStopCheckAllApsStatus) {
    CheckAndUpdateApsStatus ();
  }
}

/**
  Get Protected mode code segment from current GDT table.

  @return  Protected mode code segment value.
**/
UINT16
GetProtectedModeCS (
  VOID
  )
{
  IA32_DESCRIPTOR          GdtrDesc;
  IA32_SEGMENT_DESCRIPTOR  *GdtEntry;
  UINTN                    GdtEntryCount;
  UINT16                   Index;

  AsmReadGdtr (&GdtrDesc);
  GdtEntryCount = (GdtrDesc.Limit + 1) / sizeof (IA32_SEGMENT_DESCRIPTOR);
  GdtEntry = (IA32_SEGMENT_DESCRIPTOR *) GdtrDesc.Base;
  for (Index = 0; Index < GdtEntryCount; Index++) {
    if (GdtEntry->Bits.L == 0) {
      if (GdtEntry->Bits.Type > 8 && GdtEntry->Bits.L == 0) {
        break;
      }
    }
    GdtEntry++;
  }
  ASSERT (Index != GdtEntryCount);
  return Index * 8;
}

/**
  Do sync on APs.

  @param[in, out] Buffer  Pointer to private data buffer.
**/
VOID
EFIAPI
RelocateApLoop (
  IN OUT VOID  *Buffer
  )
{
  CPU_MP_DATA            *CpuMpData;
  BOOLEAN                MwaitSupport;
  ASM_RELOCATE_AP_LOOP   AsmRelocateApLoopFunc;
  UINTN                  ProcessorNumber;

  MpInitLibWhoAmI (&ProcessorNumber);
  CpuMpData    = GetCpuMpData ();
  MwaitSupport = IsMwaitSupport ();
  AsmRelocateApLoopFunc = (ASM_RELOCATE_AP_LOOP) (UINTN) mReservedApLoopFunc;
  AsmRelocateApLoopFunc (
    MwaitSupport,
    CpuMpData->ApTargetCState,
    CpuMpData->PmCodeSegment,
    mReservedTopOfApStack - ProcessorNumber * AP_SAFE_STACK_SIZE,
    (UINTN) &mNumberToFinish
    );
  //
  // It should never reach here
  //
  ASSERT (FALSE);
}

/**
  Callback function for ExitBootServices.

  @param[in]  Event             Event whose notification function is being invoked.
  @param[in]  Context           The pointer to the notification function's context,
                                which is implementation-dependent.

**/
VOID
EFIAPI
MpInitChangeApLoopCallback (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  CPU_MP_DATA               *CpuMpData;

  CpuMpData = GetCpuMpData ();
  CpuMpData->PmCodeSegment = GetProtectedModeCS ();
  CpuMpData->ApLoopMode = PcdGet8 (PcdCpuApLoopMode);
  mNumberToFinish = CpuMpData->CpuCount - 1;
  WakeUpAP (CpuMpData, TRUE, 0, RelocateApLoop, NULL, TRUE);
  while (mNumberToFinish > 0) {
    CpuPause ();
  }
  DEBUG ((DEBUG_INFO, "%a() done!\n", __FUNCTION__));
}

/**
  Initialize global data for MP support.

  @param[in] CpuMpData  The pointer to CPU MP Data structure.
**/
VOID
InitMpGlobalData (
  IN CPU_MP_DATA               *CpuMpData
  )
{
  EFI_STATUS                          Status;
  EFI_PHYSICAL_ADDRESS                Address;
  UINTN                               ApSafeBufferSize;
  UINTN                               Index;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR     MemDesc;
  UINTN                               StackBase;
  CPU_INFO_IN_HOB                     *CpuInfoInHob;

  SaveCpuMpData (CpuMpData);

  if (CpuMpData->CpuCount == 1) {
    //
    // If only BSP exists, return
    //
    return;
  }

  if (PcdGetBool (PcdCpuStackGuard)) {
    //
    // One extra page at the bottom of the stack is needed for Guard page.
    //
    if (CpuMpData->CpuApStackSize <= EFI_PAGE_SIZE) {
      DEBUG ((DEBUG_ERROR, "PcdCpuApStackSize is not big enough for Stack Guard!\n"));
      ASSERT (FALSE);
    }

    //
    // DXE will reuse stack allocated for APs at PEI phase if it's available.
    // Let's check it here.
    //
    // Note: BSP's stack guard is set at DxeIpl phase. But for the sake of
    // BSP/AP exchange, stack guard for ApTopOfStack of cpu 0 will still be
    // set here.
    //
    CpuInfoInHob = (CPU_INFO_IN_HOB *)(UINTN)CpuMpData->CpuInfoInHob;
    for (Index = 0; Index < CpuMpData->CpuCount; ++Index) {
      if (CpuInfoInHob != NULL && CpuInfoInHob[Index].ApTopOfStack != 0) {
        StackBase = (UINTN)CpuInfoInHob[Index].ApTopOfStack - CpuMpData->CpuApStackSize;
      } else {
        StackBase = CpuMpData->Buffer + Index * CpuMpData->CpuApStackSize;
      }

      Status = gDS->GetMemorySpaceDescriptor (StackBase, &MemDesc);
      ASSERT_EFI_ERROR (Status);

      Status = gDS->SetMemorySpaceAttributes (
                      StackBase,
                      EFI_PAGES_TO_SIZE (1),
                      MemDesc.Attributes | EFI_MEMORY_RP
                      );
      ASSERT_EFI_ERROR (Status);

      DEBUG ((DEBUG_INFO, "Stack Guard set at %lx [cpu%lu]!\n",
              (UINT64)StackBase, (UINT64)Index));
    }
  }

  //
  // Avoid APs access invalid buffer data which allocated by BootServices,
  // so we will allocate reserved data for AP loop code. We also need to
  // allocate this buffer below 4GB due to APs may be transferred to 32bit
  // protected mode on long mode DXE.
  // Allocating it in advance since memory services are not available in
  // Exit Boot Services callback function.
  //
  ApSafeBufferSize  = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (
                        CpuMpData->AddressMap.RelocateApLoopFuncSize
                        ));
  Address = BASE_4GB - 1;
  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiReservedMemoryType,
                   EFI_SIZE_TO_PAGES (ApSafeBufferSize),
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  mReservedApLoopFunc = (VOID *) (UINTN) Address;
  ASSERT (mReservedApLoopFunc != NULL);

  //
  // Make sure that the buffer memory is executable if NX protection is enabled
  // for EfiReservedMemoryType.
  //
  // TODO: Check EFI_MEMORY_XP bit set or not once it's available in DXE GCD
  //       service.
  //
  Status = gDS->GetMemorySpaceDescriptor (Address, &MemDesc);
  if (!EFI_ERROR (Status)) {
    gDS->SetMemorySpaceAttributes (
           Address,
           ApSafeBufferSize,
           MemDesc.Attributes & (~EFI_MEMORY_XP)
           );
  }

  ApSafeBufferSize = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (
                       CpuMpData->CpuCount * AP_SAFE_STACK_SIZE
                       ));
  Address = BASE_4GB - 1;
  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiReservedMemoryType,
                   EFI_SIZE_TO_PAGES (ApSafeBufferSize),
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  mReservedTopOfApStack = (UINTN) Address + ApSafeBufferSize;
  ASSERT ((mReservedTopOfApStack & (UINTN)(CPU_STACK_ALIGNMENT - 1)) == 0);
  CopyMem (
    mReservedApLoopFunc,
    CpuMpData->AddressMap.RelocateApLoopFuncAddress,
    CpuMpData->AddressMap.RelocateApLoopFuncSize
    );

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  CheckApsStatus,
                  NULL,
                  &mCheckAllApsEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Set timer to check all APs status.
  //
  Status = gBS->SetTimer (
                  mCheckAllApsEvent,
                  TimerPeriodic,
                  AP_CHECK_INTERVAL
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  MpInitChangeApLoopCallback,
                  NULL,
                  &mMpInitExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MpInitChangeApLoopCallback,
                  NULL,
                  &gEfiEventLegacyBootGuid,
                  &mLegacyBootEvent
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  This service executes a caller provided function on all enabled APs.

  @param[in]  Procedure               A pointer to the function to be run on
                                      enabled APs of the system. See type
                                      EFI_AP_PROCEDURE.
  @param[in]  SingleThread            If TRUE, then all the enabled APs execute
                                      the function specified by Procedure one by
                                      one, in ascending order of processor handle
                                      number.  If FALSE, then all the enabled APs
                                      execute the function specified by Procedure
                                      simultaneously.
  @param[in]  WaitEvent               The event created by the caller with CreateEvent()
                                      service.  If it is NULL, then execute in
                                      blocking mode. BSP waits until all APs finish
                                      or TimeoutInMicroSeconds expires.  If it's
                                      not NULL, then execute in non-blocking mode.
                                      BSP requests the function specified by
                                      Procedure to be started on all the enabled
                                      APs, and go on executing immediately. If
                                      all return from Procedure, or TimeoutInMicroSeconds
                                      expires, this event is signaled. The BSP
                                      can use the CheckEvent() or WaitForEvent()
                                      services to check the state of event.  Type
                                      EFI_EVENT is defined in CreateEvent() in
                                      the Unified Extensible Firmware Interface
                                      Specification.
  @param[in]  TimeoutInMicroseconds   Indicates the time limit in microseconds for
                                      APs to return from Procedure, either for
                                      blocking or non-blocking mode. Zero means
                                      infinity.  If the timeout expires before
                                      all APs return from Procedure, then Procedure
                                      on the failed APs is terminated. All enabled
                                      APs are available for next function assigned
                                      by MpInitLibStartupAllAPs() or
                                      MPInitLibStartupThisAP().
                                      If the timeout expires in blocking mode,
                                      BSP returns EFI_TIMEOUT.  If the timeout
                                      expires in non-blocking mode, WaitEvent
                                      is signaled with SignalEvent().
  @param[in]  ProcedureArgument       The parameter passed into Procedure for
                                      all APs.
  @param[out] FailedCpuList           If NULL, this parameter is ignored. Otherwise,
                                      if all APs finish successfully, then its
                                      content is set to NULL. If not all APs
                                      finish before timeout expires, then its
                                      content is set to address of the buffer
                                      holding handle numbers of the failed APs.
                                      The buffer is allocated by MP Initialization
                                      library, and it's the caller's responsibility to
                                      free the buffer with FreePool() service.
                                      In blocking mode, it is ready for consumption
                                      when the call returns. In non-blocking mode,
                                      it is ready when WaitEvent is signaled.  The
                                      list of failed CPU is terminated by
                                      END_OF_CPU_LIST.

  @retval EFI_SUCCESS             In blocking mode, all APs have finished before
                                  the timeout expired.
  @retval EFI_SUCCESS             In non-blocking mode, function has been dispatched
                                  to all enabled APs.
  @retval EFI_UNSUPPORTED         A non-blocking mode request was made after the
                                  UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was
                                  signaled.
  @retval EFI_UNSUPPORTED         WaitEvent is not NULL if non-blocking mode is not
                                  supported.
  @retval EFI_DEVICE_ERROR        Caller processor is AP.
  @retval EFI_NOT_STARTED         No enabled APs exist in the system.
  @retval EFI_NOT_READY           Any enabled APs are busy.
  @retval EFI_NOT_READY           MP Initialize Library is not initialized.
  @retval EFI_TIMEOUT             In blocking mode, the timeout expired before
                                  all enabled APs have finished.
  @retval EFI_INVALID_PARAMETER   Procedure is NULL.

**/
EFI_STATUS
EFIAPI
MpInitLibStartupAllAPs (
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  BOOLEAN                   SingleThread,
  IN  EFI_EVENT                 WaitEvent               OPTIONAL,
  IN  UINTN                     TimeoutInMicroseconds,
  IN  VOID                      *ProcedureArgument      OPTIONAL,
  OUT UINTN                     **FailedCpuList         OPTIONAL
  )
{
  EFI_STATUS              Status;

  //
  // Temporarily stop checkAllApsStatus for avoid resource dead-lock.
  //
  mStopCheckAllApsStatus = TRUE;

  Status = StartupAllAPsWorker (
             Procedure,
             SingleThread,
             WaitEvent,
             TimeoutInMicroseconds,
             ProcedureArgument,
             FailedCpuList
             );

  //
  // Start checkAllApsStatus
  //
  mStopCheckAllApsStatus = FALSE;

  return Status;
}

/**
  This service lets the caller get one enabled AP to execute a caller-provided
  function.

  @param[in]  Procedure               A pointer to the function to be run on the
                                      designated AP of the system. See type
                                      EFI_AP_PROCEDURE.
  @param[in]  ProcessorNumber         The handle number of the AP. The range is
                                      from 0 to the total number of logical
                                      processors minus 1. The total number of
                                      logical processors can be retrieved by
                                      MpInitLibGetNumberOfProcessors().
  @param[in]  WaitEvent               The event created by the caller with CreateEvent()
                                      service.  If it is NULL, then execute in
                                      blocking mode. BSP waits until this AP finish
                                      or TimeoutInMicroSeconds expires.  If it's
                                      not NULL, then execute in non-blocking mode.
                                      BSP requests the function specified by
                                      Procedure to be started on this AP,
                                      and go on executing immediately. If this AP
                                      return from Procedure or TimeoutInMicroSeconds
                                      expires, this event is signaled. The BSP
                                      can use the CheckEvent() or WaitForEvent()
                                      services to check the state of event.  Type
                                      EFI_EVENT is defined in CreateEvent() in
                                      the Unified Extensible Firmware Interface
                                      Specification.
  @param[in]  TimeoutInMicroseconds   Indicates the time limit in microseconds for
                                      this AP to finish this Procedure, either for
                                      blocking or non-blocking mode. Zero means
                                      infinity.  If the timeout expires before
                                      this AP returns from Procedure, then Procedure
                                      on the AP is terminated. The
                                      AP is available for next function assigned
                                      by MpInitLibStartupAllAPs() or
                                      MpInitLibStartupThisAP().
                                      If the timeout expires in blocking mode,
                                      BSP returns EFI_TIMEOUT.  If the timeout
                                      expires in non-blocking mode, WaitEvent
                                      is signaled with SignalEvent().
  @param[in]  ProcedureArgument       The parameter passed into Procedure on the
                                      specified AP.
  @param[out] Finished                If NULL, this parameter is ignored.  In
                                      blocking mode, this parameter is ignored.
                                      In non-blocking mode, if AP returns from
                                      Procedure before the timeout expires, its
                                      content is set to TRUE. Otherwise, the
                                      value is set to FALSE. The caller can
                                      determine if the AP returned from Procedure
                                      by evaluating this value.

  @retval EFI_SUCCESS             In blocking mode, specified AP finished before
                                  the timeout expires.
  @retval EFI_SUCCESS             In non-blocking mode, the function has been
                                  dispatched to specified AP.
  @retval EFI_UNSUPPORTED         A non-blocking mode request was made after the
                                  UEFI event EFI_EVENT_GROUP_READY_TO_BOOT was
                                  signaled.
  @retval EFI_UNSUPPORTED         WaitEvent is not NULL if non-blocking mode is not
                                  supported.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_TIMEOUT             In blocking mode, the timeout expired before
                                  the specified AP has finished.
  @retval EFI_NOT_READY           The specified AP is busy.
  @retval EFI_NOT_READY           MP Initialize Library is not initialized.
  @retval EFI_NOT_FOUND           The processor with the handle specified by
                                  ProcessorNumber does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP or disabled AP.
  @retval EFI_INVALID_PARAMETER   Procedure is NULL.

**/
EFI_STATUS
EFIAPI
MpInitLibStartupThisAP (
  IN  EFI_AP_PROCEDURE          Procedure,
  IN  UINTN                     ProcessorNumber,
  IN  EFI_EVENT                 WaitEvent               OPTIONAL,
  IN  UINTN                     TimeoutInMicroseconds,
  IN  VOID                      *ProcedureArgument      OPTIONAL,
  OUT BOOLEAN                   *Finished               OPTIONAL
  )
{
  EFI_STATUS              Status;

  //
  // temporarily stop checkAllApsStatus for avoid resource dead-lock.
  //
  mStopCheckAllApsStatus = TRUE;

  Status = StartupThisAPWorker (
             Procedure,
             ProcessorNumber,
             WaitEvent,
             TimeoutInMicroseconds,
             ProcedureArgument,
             Finished
             );

  mStopCheckAllApsStatus = FALSE;

  return Status;
}

/**
  This service switches the requested AP to be the BSP from that point onward.
  This service changes the BSP for all purposes. This call can only be performed
  by the current BSP.

  @param[in] ProcessorNumber   The handle number of AP that is to become the new
                               BSP. The range is from 0 to the total number of
                               logical processors minus 1. The total number of
                               logical processors can be retrieved by
                               MpInitLibGetNumberOfProcessors().
  @param[in] EnableOldBSP      If TRUE, then the old BSP will be listed as an
                               enabled AP. Otherwise, it will be disabled.

  @retval EFI_SUCCESS             BSP successfully switched.
  @retval EFI_UNSUPPORTED         Switching the BSP cannot be completed prior to
                                  this service returning.
  @retval EFI_UNSUPPORTED         Switching the BSP is not supported.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_NOT_FOUND           The processor with the handle specified by
                                  ProcessorNumber does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the current BSP or
                                  a disabled AP.
  @retval EFI_NOT_READY           The specified AP is busy.
  @retval EFI_NOT_READY           MP Initialize Library is not initialized.

**/
EFI_STATUS
EFIAPI
MpInitLibSwitchBSP (
  IN UINTN                     ProcessorNumber,
  IN BOOLEAN                   EnableOldBSP
  )
{
  EFI_STATUS                   Status;
  EFI_TIMER_ARCH_PROTOCOL      *Timer;
  UINT64                       TimerPeriod;

  TimerPeriod = 0;
  //
  // Locate Timer Arch Protocol
  //
  Status = gBS->LocateProtocol (&gEfiTimerArchProtocolGuid, NULL, (VOID **) &Timer);
  if (EFI_ERROR (Status)) {
    Timer = NULL;
  }

  if (Timer != NULL) {
    //
    // Save current rate of DXE Timer
    //
    Timer->GetTimerPeriod (Timer, &TimerPeriod);
    //
    // Disable DXE Timer and drain pending interrupts
    //
    Timer->SetTimerPeriod (Timer, 0);
  }

  Status = SwitchBSPWorker (ProcessorNumber, EnableOldBSP);

  if (Timer != NULL) {
    //
    // Enable and restore rate of DXE Timer
    //
    Timer->SetTimerPeriod (Timer, TimerPeriod);
  }

  return Status;
}

/**
  This service lets the caller enable or disable an AP from this point onward.
  This service may only be called from the BSP.

  @param[in] ProcessorNumber   The handle number of AP.
                               The range is from 0 to the total number of
                               logical processors minus 1. The total number of
                               logical processors can be retrieved by
                               MpInitLibGetNumberOfProcessors().
  @param[in] EnableAP          Specifies the new state for the processor for
                               enabled, FALSE for disabled.
  @param[in] HealthFlag        If not NULL, a pointer to a value that specifies
                               the new health status of the AP. This flag
                               corresponds to StatusFlag defined in
                               EFI_MP_SERVICES_PROTOCOL.GetProcessorInfo(). Only
                               the PROCESSOR_HEALTH_STATUS_BIT is used. All other
                               bits are ignored.  If it is NULL, this parameter
                               is ignored.

  @retval EFI_SUCCESS             The specified AP was enabled or disabled successfully.
  @retval EFI_UNSUPPORTED         Enabling or disabling an AP cannot be completed
                                  prior to this service returning.
  @retval EFI_UNSUPPORTED         Enabling or disabling an AP is not supported.
  @retval EFI_DEVICE_ERROR        The calling processor is an AP.
  @retval EFI_NOT_FOUND           Processor with the handle specified by ProcessorNumber
                                  does not exist.
  @retval EFI_INVALID_PARAMETER   ProcessorNumber specifies the BSP.
  @retval EFI_NOT_READY           MP Initialize Library is not initialized.

**/
EFI_STATUS
EFIAPI
MpInitLibEnableDisableAP (
  IN  UINTN                     ProcessorNumber,
  IN  BOOLEAN                   EnableAP,
  IN  UINT32                    *HealthFlag OPTIONAL
  )
{
  EFI_STATUS     Status;
  BOOLEAN        TempStopCheckState;

  TempStopCheckState = FALSE;
  //
  // temporarily stop checkAllAPsStatus for initialize parameters.
  //
  if (!mStopCheckAllApsStatus) {
    mStopCheckAllApsStatus = TRUE;
    TempStopCheckState     = TRUE;
  }

  Status = EnableDisableApWorker (ProcessorNumber, EnableAP, HealthFlag);

  if (TempStopCheckState) {
    mStopCheckAllApsStatus = FALSE;
  }

  return Status;
}
