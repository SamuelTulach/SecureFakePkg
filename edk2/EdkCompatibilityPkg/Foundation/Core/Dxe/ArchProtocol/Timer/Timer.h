/*++

Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  Timer.h

Abstract:

  Timer Architectural Protocol as defined in the DXE CIS

  This code is used to provide the timer tick for the DXE core.

--*/

#ifndef _ARCH_PROTOCOL_TIMER_H_
#define _ARCH_PROTOCOL_TIMER_H_

//
// Global ID for the Timer Architectural Protocol
//
#define EFI_TIMER_ARCH_PROTOCOL_GUID \
  { 0x26baccb3, 0x6f42, 0x11d4, {0xbc, 0xe7, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81} }

//
// Declare forward reference for the Timer Architectural Protocol
//
EFI_FORWARD_DECLARATION (EFI_TIMER_ARCH_PROTOCOL);

typedef
VOID
(EFIAPI *EFI_TIMER_NOTIFY) (
  IN UINT64  Time
  );
/*++

Routine Description:

  This function of this type is called when a timer interrupt fires.  This 
  function executes at TPL_HIGH_LEVEL.  The DXE Core will register a funtion
  of tyis type to be called for the timer interrupt, so it can know how much 
  time has passed.  This information is used to signal timer based events.  

Arguments:

  Time - Time since the last timer interrupt in 100 ns units.  This will 
         typically be TimerPeriod, but if a timer interrupt is missed, and the 
         EFI_TIMER_ARCH_PROTOCOL driver can detect missed interrupts, then Time 
         will contain the actual amount of time since the last interrupt.

Returns: 

  None.

--*/

typedef 
EFI_STATUS
(EFIAPI *EFI_TIMER_REGISTER_HANDLER) (
  IN EFI_TIMER_ARCH_PROTOCOL    *This,
  IN EFI_TIMER_NOTIFY           NotifyFunction
);
/*++

Routine Description:

  This function registers the handler NotifyFunction so it is called every time 
  the timer interrupt fires.  It also passes the amount of time since the last 
  handler call to the NotifyFunction.  If NotifyFunction is NULL, then the 
  handler is unregistered.  If the handler is registered, then EFI_SUCCESS is 
  returned.  If the CPU does not support registering a timer interrupt handler, 
  then EFI_UNSUPPORTED is returned.  If an attempt is made to register a handler 
  when a handler is already registered, then EFI_ALREADY_STARTED is returned.  
  If an attempt is made to unregister a handler when a handler is not registered, 
  then EFI_INVALID_PARAMETER is returned.  If an error occurs attempting to 
  register the NotifyFunction with the timer interrupt, then EFI_DEVICE_ERROR 
  is returned.

Arguments:

  This           - The EFI_TIMER_ARCH_PROTOCOL instance.

  NotifyFunction - The function to call when a timer interrupt fires.  This 
                   function executes at TPL_HIGH_LEVEL.  The DXE Core will 
                   register a handler for the timer interrupt, so it can know 
                   how much time has passed.  This information is used to 
                   signal timer based events.  NULL will unregister the handler.

Returns: 

  EFI_SUCCESS           - The timer handler was registered.

  EFI_UNSUPPORTED       - The platform does not support timer interrupts.

  EFI_ALREADY_STARTED   - NotifyFunction is not NULL, and a handler is already 
                          registered.

  EFI_INVALID_PARAMETER - NotifyFunction is NULL, and a handler was not 
                          previously registered.

  EFI_DEVICE_ERROR      - The timer handler could not be registered.

--*/

typedef 
EFI_STATUS
(EFIAPI *EFI_TIMER_SET_TIMER_PERIOD) (
  IN EFI_TIMER_ARCH_PROTOCOL    *This,
  IN UINT64                     TimerPeriod
  );
/*++

Routine Description:

  This function adjusts the period of timer interrupts to the value specified 
  by TimerPeriod.  If the timer period is updated, then the selected timer 
  period is stored in EFI_TIMER.TimerPeriod, and EFI_SUCCESS is returned.  If 
  the timer hardware is not programmable, then EFI_UNSUPPORTED is returned.  
  If an error occurs while attempting to update the timer period, then the 
  timer hardware will be put back in its state prior to this call, and 
  EFI_DEVICE_ERROR is returned.  If TimerPeriod is 0, then the timer interrupt 
  is disabled.  This is not the same as disabling the CPU's interrupts.  
  Instead, it must either turn off the timer hardware, or it must adjust the 
  interrupt controller so that a CPU interrupt is not generated when the timer 
  interrupt fires. 

Arguments:

  This        - The EFI_TIMER_ARCH_PROTOCOL instance.

  TimerPeriod - The rate to program the timer interrupt in 100 nS units.  If 
                the timer hardware is not programmable, then EFI_UNSUPPORTED is 
                returned.  If the timer is programmable, then the timer period 
                will be rounded up to the nearest timer period that is supported 
                by the timer hardware.  If TimerPeriod is set to 0, then the 
                timer interrupts will be disabled.

Returns: 

  EFI_SUCCESS      - The timer period was changed.

  EFI_UNSUPPORTED  - The platform cannot change the period of the timer interrupt.

  EFI_DEVICE_ERROR - The timer period could not be changed due to a device error.

--*/

typedef 
EFI_STATUS
(EFIAPI *EFI_TIMER_GET_TIMER_PERIOD) (
  IN EFI_TIMER_ARCH_PROTOCOL    *This,
  OUT UINT64                     *TimerPeriod
  );
/*++

Routine Description:

  This function retrieves the period of timer interrupts in 100 ns units, 
  returns that value in TimerPeriod, and returns EFI_SUCCESS.  If TimerPeriod 
  is NULL, then EFI_INVALID_PARAMETER is returned.  If a TimerPeriod of 0 is 
  returned, then the timer is currently disabled.

Arguments:

  This        - The EFI_TIMER_ARCH_PROTOCOL instance.

  TimerPeriod - A pointer to the timer period to retrieve in 100 ns units.  If 
                0 is returned, then the timer is currently disabled.

Returns: 

  EFI_SUCCESS           - The timer period was returned in TimerPeriod.

  EFI_INVALID_PARAMETER - TimerPeriod is NULL.

--*/

typedef 
EFI_STATUS
(EFIAPI *EFI_TIMER_GENERATE_SOFT_INTERRUPT) (
  IN EFI_TIMER_ARCH_PROTOCOL    *This
  );
/*++

Routine Description:

  This function generates a soft timer interrupt. If the platform does not support soft 
  timer interrupts, then EFI_UNSUPPORTED is returned. Otherwise, EFI_SUCCESS is returned. 
  If a handler has been registered through the EFI_TIMER_ARCH_PROTOCOL.RegisterHandler() 
  service, then a soft timer interrupt will be generated. If the timer interrupt is 
  enabled when this service is called, then the registered handler will be invoked. The 
  registered handler should not be able to distinguish a hardware-generated timer 
  interrupt from a software-generated timer interrupt.

Arguments:

  This  -  The EFI_TIMER_ARCH_PROTOCOL instance.

Returns: 

  EFI_SUCCESS       - The soft timer interrupt was generated.

  EFI_UNSUPPORTED   - The platform does not support the generation of soft timer interrupts.

--*/

//
// Interface stucture for the Timer Architectural Protocol
//
struct _EFI_TIMER_ARCH_PROTOCOL {
  EFI_TIMER_REGISTER_HANDLER          RegisterHandler;
  EFI_TIMER_SET_TIMER_PERIOD          SetTimerPeriod;
  EFI_TIMER_GET_TIMER_PERIOD          GetTimerPeriod;
  EFI_TIMER_GENERATE_SOFT_INTERRUPT   GenerateSoftInterrupt;
};

/*++

  Protocol Description:
    This protocol provides the services to initialize a periodic timer 
    interrupt, and to register a handler that is called each time the timer
    interrupt fires.  It may also provide a service to adjust the rate of the
    periodic timer interrupt.  When a timer interrupt occurs, the handler is 
    passed the amount of time that has passed since the previous timer 
    interrupt.
  
  Parameters:
    
    RegisterHandler - Registers a handler that will be called each time the 
                      timer interrupt fires.  TimerPeriod defines the minimum 
                      time between timer interrupts, so TimerPeriod will also 
                      be the minimum time between calls to the registered 
                      handler.
    
    SetTimerPeriod  - Sets the period of the timer interrupt in 100 nS units.  
                      This function is optional, and may return EFI_UNSUPPORTED.  
                      If this function is supported, then the timer period will 
                      be rounded up to the nearest supported timer period.
    
    GetTimerPeriod  - Retrieves the period of the timer interrupt in 100 nS units.
    
    GenerateSoftInterrupt - 
                      Generates a soft timer interrupt that simulates the firing of 
                      the timer interrupt. This service can be used to invoke the 
                      registered handler if the timer interrupt has been masked for 
                      a period of time.

--*/

extern EFI_GUID gEfiTimerArchProtocolGuid;

#endif
