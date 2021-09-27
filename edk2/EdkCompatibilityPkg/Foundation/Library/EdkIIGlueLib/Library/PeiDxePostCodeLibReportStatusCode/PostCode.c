/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.   
  
  
Module Name:

  PostCode.c
  
Abstract: 

  Post Code Lib

--*/

#include "EdkIIGlueDxe.h"

/**
  Converts POST code value to status code value.

  This macro converts the post code to status code value. Bits 0..4 of PostCode 
  are mapped to bits 16..20 of status code value, and bits 5..7 of PostCode are mapped to bits 
  24..26 of status code value.  

  @param  PostCode  POST code value. 

  @return The converted status code value.

**/
#define POST_CODE_TO_STATUS_CODE_VALUE(PostCode)  \
  ((EFI_STATUS_CODE_VALUE) (((PostCode & 0x1f) << 16) | ((PostCode & 0x3) << 19)))

/**
  Sends an 32-bit value to a POST card.

  Sends the 32-bit value specified by Value to a POST card, and returns Value.  
  Some implementations of this library function may perform I/O operations 
  directly to a POST card device.  Other implementations may send Value to 
  ReportStatusCode(), and the status code reporting mechanism will eventually 
  display the 32-bit value on the status reporting device.
  
  PostCode() must actively prevent recursion.  If PostCode() is called while 
  processing another any other Report Status Code Library function, then 
  PostCode() must return Value immediately.

  @param  Value  The 32-bit value to write to the POST card.

  @return  Value

**/
UINT32
EFIAPI
GluePostCode (
  IN UINT32  Value
  )
{
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, POST_CODE_TO_STATUS_CODE_VALUE (Value));
  return Value;
}


/**
  Sends an 32-bit value to a POST and associated ASCII string.

  Sends the 32-bit value specified by Value to a POST card, and returns Value.
  If Description is not NULL, then the ASCII string specified by Description is 
  also passed to the handler that displays the POST card value.  Some 
  implementations of this library function may perform I/O operations directly 
  to a POST card device.  Other implementations may send Value to ReportStatusCode(), 
  and the status code reporting mechanism will eventually display the 32-bit 
  value on the status reporting device.  

  PostCodeWithDescription()must actively prevent recursion.  If 
  PostCodeWithDescription() is called while processing another any other Report 
  Status Code Library function, then PostCodeWithDescription() must return Value 
  immediately.

  @param  Value        The 32-bit value to write to the POST card.
  @param  Description  Pointer to an ASCII string that is a description of the 
                       POST code value.  This is an optional parameter that may 
                       be NULL.

  @return  Value

**/
UINT32
EFIAPI
GluePostCodeWithDescription (
  IN UINT32       Value,
  IN CONST CHAR8  *Description  OPTIONAL
  )
{
  if (Description == NULL) { 
    REPORT_STATUS_CODE (
      EFI_PROGRESS_CODE,
      POST_CODE_TO_STATUS_CODE_VALUE (Value)
      );
  } else {
    REPORT_STATUS_CODE_WITH_EXTENDED_DATA (
      EFI_PROGRESS_CODE,
      POST_CODE_TO_STATUS_CODE_VALUE (Value),
      Description,
      AsciiStrSize (Description)
      );
  }

  return Value;
}


/**
  Returns TRUE if POST Codes are enabled.

  This function returns TRUE if the POST_CODE_PROPERTY_POST_CODE_ENABLED 
  bit of PcdPostCodePropertyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The POST_CODE_PROPERTY_POST_CODE_ENABLED bit of 
                  PcdPostCodeProperyMask is set.
  @retval  FALSE  The POST_CODE_PROPERTY_POST_CODE_ENABLED bit of 
                  PcdPostCodeProperyMask is clear.

**/
BOOLEAN
EFIAPI
GluePostCodeEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8(PcdPostCodePropertyMask) & POST_CODE_PROPERTY_POST_CODE_ENABLED) != 0);
}


/**
  Returns TRUE if POST code descriptions are enabled.

  This function returns TRUE if the 
  POST_CODE_PROPERTY_POST_CODE_ENABLED bit of 
  PcdPostCodePropertyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The POST_CODE_PROPERTY_POST_CODE_ENABLED 
                  bit of PcdPostCodeProperyMask is set.
  @retval  FALSE  The POST_CODE_PROPERTY_POST_CODE_ENABLED 
                  bit of PcdPostCodeProperyMask is clear.

**/
BOOLEAN
EFIAPI
GluePostCodeDescriptionEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8(PcdPostCodePropertyMask) & POST_CODE_PROPERTY_POST_CODE_ENABLED) != 0);
}
