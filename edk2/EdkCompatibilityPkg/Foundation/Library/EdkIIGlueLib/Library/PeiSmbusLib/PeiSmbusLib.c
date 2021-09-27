/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.              


Module Name:

  PeiSmbusLib.c
  
Abstract: 

  PEI Smbus Lib internal functions

--*/

#include "PeiSmbusLibInternal.h"

/**
  Gets Smbus PPIs.

  This internal function retrieves Smbus PPI from PPI database.

  @param  PeiServices   An indirect pointer to the EFI_PEI_SERVICES published by the PEI Foundation.

  @return The pointer to Smbus PPI.

**/
EFI_PEI_SMBUS_PPI *
InternalGetSmbusPpi (
  EFI_PEI_SERVICES      **PeiServices
  ) 
{
  EFI_STATUS            Status;
  EFI_PEI_SMBUS_PPI     *SmbusPpi;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiSmbusPpiGuid, 0, NULL, (VOID **) &SmbusPpi);
  ASSERT_EFI_ERROR (Status);
  ASSERT (SmbusPpi != NULL);

  return SmbusPpi;
}

/**
  Executes an SMBus operation to an SMBus controller. 

  This function provides a standard way to execute Smbus script
  as defined in the SmBus Specification. The data can either be of
  the Length byte, word, or a block of data.

  @param  SmbusOperation  Signifies which particular SMBus hardware protocol instance that it will use to
                          execute the SMBus transactions.
  @param  SmBusAddress    Address that encodes the SMBUS Slave Address,
                          SMBUS Command, SMBUS Data Length, and PEC.
  @param  Length          Signifies the number of bytes that this operation will do. The maximum number of
                          bytes can be revision specific and operation specific.
  @param  Buffer          Contains the value of data to execute to the SMBus slave device. Not all operations
                          require this argument. The length of this buffer is identified by Length.
  @param  Status          Return status for the executed command.
                          This is an optional parameter and may be NULL.

  @return The actual number of bytes that are executed for this operation..

**/
UINTN
InternalSmBusExec (
  IN     EFI_SMBUS_OPERATION        SmbusOperation,
  IN     UINTN                      SmBusAddress,
  IN     UINTN                      Length,
  IN OUT VOID                       *Buffer,
     OUT RETURN_STATUS              *Status        OPTIONAL
  )
{
  EFI_PEI_SMBUS_PPI         *SmbusPpi;
  EFI_PEI_SERVICES          **PeiServices;
  RETURN_STATUS             ReturnStatus;
  EFI_SMBUS_DEVICE_ADDRESS  SmbusDeviceAddress;

  PeiServices = GetPeiServicesTablePointer ();
  SmbusPpi    = InternalGetSmbusPpi (PeiServices);
  SmbusDeviceAddress.SmbusDeviceAddress = SMBUS_LIB_SLAVE_ADDRESS (SmBusAddress);

  ReturnStatus = SmbusPpi->Execute (
                             PeiServices,
                             SmbusPpi,
                             SmbusDeviceAddress,
                             SMBUS_LIB_COMMAND (SmBusAddress),
                             SmbusOperation,
                             SMBUS_LIB_PEC (SmBusAddress),  
                             &Length,
                             Buffer
                             );
  if (Status != NULL) {
    *Status = ReturnStatus;
  }

  return Length;
}
