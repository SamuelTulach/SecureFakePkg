/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  EdkIIGlueReportStatusCodeLib.h
  
Abstract: 

  Public header file for Report Status Code Lib

--*/

#ifndef __EDKII_GLUE_REPORT_STATUS_CODE_LIB_H__
#define __EDKII_GLUE_REPORT_STATUS_CODE_LIB_H__



#define CodeTypeToPostCode(_CODETYPE, _VALUE, _POSTCODE)    GlueCodeTypeToPostCode(_CODETYPE, _VALUE, _POSTCODE)
#define ReportStatusCodeExtractAssertInfo(_CODETYPE, _VALUE, _DATA, _FILENAME, _DESC, _LINENUMBER)  \
                                                            GlueReportStatusCodeExtractAssertInfo(_CODETYPE, _VALUE, _DATA, _FILENAME, _DESC, _LINENUMBER)
#define ReportStatusCodeExtractDebugInfo(_DATA, _ERRORLEVEL, _MARKER, _FORMAT)                      \
                                                            GlueReportStatusCodeExtractDebugInfo(_DATA, _ERRORLEVEL, _MARKER, _FORMAT)
#define ReportStatusCode(_TYPE, _VALUE)                     GlueReportStatusCode(_TYPE, _VALUE)
#define ReportStatusCodeWithDevicePath(_TYPE, _VALUE, _DEVICEPATH)                                  \
                                                            GlueReportStatusCodeWithDevicePath(_TYPE, _VALUE, _DEVICEPATH)
#define ReportStatusCodeWithExtendedData(_TYPE, _VALUE, _EXDATA, _EXDATASIZE)                       \
                                                            GlueReportStatusCodeWithExtendedData(_TYPE, _VALUE, _EXDATA, _EXDATASIZE)
#define ReportStatusCodeEx(_TYPE, _VALUE, _INSTANCE, _CALLERID, _EXDATAGUID, _EXDATA, _EXDATASIZE)  \
                                                            GlueReportStatusCodeEx(_TYPE, _VALUE, _INSTANCE, _CALLERID, _EXDATAGUID, _EXDATA, _EXDATASIZE)
#define ReportProgressCodeEnabled()                         GlueReportProgressCodeEnabled()
#define ReportErrorCodeEnabled()                            GlueReportErrorCodeEnabled()
#define ReportDebugCodeEnabled()                            GlueReportDebugCodeEnabled()


//
// Declare bits for PcdReportStatusCodePropertyMask
//
#define REPORT_STATUS_CODE_PROPERTY_PROGRESS_CODE_ENABLED          0x00000001
#define REPORT_STATUS_CODE_PROPERTY_ERROR_CODE_ENABLED             0x00000002
#define REPORT_STATUS_CODE_PROPERTY_DEBUG_CODE_ENABLED             0x00000004

//
// Extended Data structure definitions with EFI_STATUS_CODE_DATA headers removed
//

///
/// Voltage Extended Error Data
///
typedef struct {
  EFI_EXP_BASE10_DATA   Voltage;
  EFI_EXP_BASE10_DATA   Threshold;
} REPORT_STATUS_CODE_LIBRARY_COMPUTING_UNIT_VOLTAGE_ERROR_DATA;

///
/// Microcode Update Extended Error Data
///
typedef struct {
  UINT32                Version;
} REPORT_STATUS_CODE_LIBRARY_COMPUTING_UNIT_MICROCODE_UPDATE_ERROR_DATA;

///
/// Asynchronous Timer Extended Error Data
///
typedef struct {
  EFI_EXP_BASE10_DATA   TimerLimit;
} REPORT_STATUS_CODE_LIBRARY_COMPUTING_UNIT_TIMER_EXPIRED_ERROR_DATA;

///
/// Host Processor Mismatch Extended Error Data
///
typedef struct {
  UINT32                Instance;
  UINT16                Attributes;
} REPORT_STATUS_CODE_LIBRARY_HOST_PROCESSOR_MISMATCH_ERROR_DATA;

///
/// Thermal Extended Error Data
///
typedef struct {
  EFI_EXP_BASE10_DATA   Temperature;
  EFI_EXP_BASE10_DATA   Threshold;
} REPORT_STATUS_CODE_LIBRARY_COMPUTING_UNIT_THERMAL_ERROR_DATA;

///
/// Processor Disabled Extended Error Data
///
typedef struct {
  UINT32                Cause;
  BOOLEAN               SoftwareDisabled;
} REPORT_STATUS_CODE_LIBRARY_COMPUTING_UNIT_CPU_DISABLED_ERROR_DATA;

///
/// Embedded cache init extended data
///
typedef struct {
  UINT32                Level;
  EFI_INIT_CACHE_TYPE   Type;
} REPORT_STATUS_CODE_LIBRARY_CACHE_INIT_DATA;

///
/// Memory Extended Error Data
///
typedef struct {
  EFI_MEMORY_ERROR_GRANULARITY  Granularity;
  EFI_MEMORY_ERROR_OPERATION    Operation;
  UINTN                         Syndrome;
  EFI_PHYSICAL_ADDRESS          Address;
  UINTN                         Resolution;
} REPORT_STATUS_CODE_LIBRARY_MEMORY_EXTENDED_ERROR_DATA;

///
/// DIMM number
///
typedef struct {
  UINT16                Array;
  UINT16                Device;
} REPORT_STATUS_CODE_LIBRARY_STATUS_CODE_DIMM_NUMBER;

///
/// Memory Module Mismatch Extended Error Data
///
typedef struct {
  EFI_STATUS_CODE_DIMM_NUMBER Instance;
} REPORT_STATUS_CODE_LIBRARY_MEMORY_MODULE_MISMATCH_ERROR_DATA;

///
/// Memory Range Extended Data
///
typedef struct {
  EFI_PHYSICAL_ADDRESS  Start;
  EFI_PHYSICAL_ADDRESS  Length;
} REPORT_STATUS_CODE_LIBRARY_MEMORY_RANGE_EXTENDED_DATA;

///
/// Device handle Extended Data. Used for many
/// errors and progress codes to point to the device.
///
typedef struct {
  EFI_HANDLE            Handle;
} REPORT_STATUS_CODE_LIBRARY_DEVICE_HANDLE_EXTENDED_DATA;

typedef struct {
  UINT8                                *DevicePath;
} REPORT_STATUS_CODE_LIBRARY_DEVICE_PATH_EXTENDED_DATA;

typedef struct {
  EFI_HANDLE                     ControllerHandle;
  EFI_HANDLE                     DriverBindingHandle;
  UINT16                         DevicePathSize;
  UINT8                          *RemainingDevicePath;
} REPORT_STATUS_CODE_LIBRARY_STATUS_CODE_START_EXTENDED_DATA;

///
/// Resource Allocation Failure Extended Error Data
///
typedef struct {
  UINT32                             Bar;
  UINT16                             DevicePathSize;
  UINT16                             ReqResSize;
  UINT16                             AllocResSize;
  UINT8                              *DevicePath;
  UINT8                              *ReqRes;
  UINT8                              *AllocRes;
} REPORT_STATUS_CODE_LIBRARY_RESOURCE_ALLOC_FAILURE_ERROR_DATA;

///
/// Extended Error Data for Assert
///
typedef struct {
  UINT32                      LineNumber;
  UINT32                      FileNameSize;
  EFI_STATUS_CODE_STRING_DATA *FileName;
} REPORT_STATUS_CODE_LIBRARY_DEBUG_ASSERT_DATA;

///
/// System Context Data EBC/IA32/IPF
///
typedef struct {
  EFI_STATUS_CODE_EXCEP_SYSTEM_CONTEXT  Context;
} REPORT_STATUS_CODE_LIBRARY_STATUS_CODE_EXCEP_EXTENDED_DATA;

///
/// Legacy Oprom extended data
///
typedef struct {
  EFI_HANDLE            DeviceHandle;
  EFI_PHYSICAL_ADDRESS  RomImageBase;
} REPORT_STATUS_CODE_LIBRARY_LEGACY_OPROM_EXTENDED_DATA;

//
// Extern for the modules Caller ID GUID
//
extern EFI_GUID gEfiCallerIdGuid;

/**
  Converts a status code to an 8-bit POST code value.

  Converts the status code specified by CodeType and Value to an 8-bit POST code 
  and returns the 8-bit POST code in PostCode.  If CodeType is an 
  EFI_PROGRESS_CODE or CodeType is an EFI_ERROR_CODE, then bits 0..4 of PostCode 
  are set to bits 16..20 of Value, and bits 5..7 of PostCode are set to bits 
  24..26 of Value., and TRUE is returned.  Otherwise, FALSE is returned.  

  If PostCode is NULL, then ASSERT().

  @param  CodeType  The type of status code being converted.
  @param  Value     The status code value being converted.
  @param  PostCode  A pointer to the 8-bit POST code value to return. 

  @retval  TRUE   The status code specified by CodeType and Value was converted 
                  to an 8-bit POST code and returned in  PostCode.
  @retval  FALSE  The status code specified by CodeType and Value could not be 
                  converted to an 8-bit POST code value.

**/
BOOLEAN
EFIAPI
GlueCodeTypeToPostCode (
  IN  EFI_STATUS_CODE_TYPE   CodeType,
  IN  EFI_STATUS_CODE_VALUE  Value,
  OUT UINT8                  *PostCode
  );


/**
  Extracts ASSERT() information from a status code structure.

  Converts the status code specified by CodeType, Value, and Data to the ASSERT()
  arguments specified by Filename, Description, and LineNumber.  If CodeType is 
  an EFI_ERROR_CODE, and CodeType has a severity of EFI_ERROR_UNRECOVERED, and 
  Value has an operation mask of EFI_SW_EC_ILLEGAL_SOFTWARE_STATE, extract 
  Filename, Description, and LineNumber from the optional data area of the 
  status code buffer specified by Data.  The optional data area of Data contains 
  a Null-terminated ASCII string for the FileName, followed by a Null-terminated 
  ASCII string for the Description, followed by a 32-bit LineNumber.  If the 
  ASSERT() information could be extracted from Data, then return TRUE.  
  Otherwise, FALSE is returned.  

  If Data is NULL, then ASSERT().
  If Filename is NULL, then ASSERT().
  If Description is NULL, then ASSERT().
  If LineNumber is NULL, then ASSERT().

  @param  CodeType     The type of status code being converted.
  @param  Value        The status code value being converted.
  @param  Data         Pointer to status code data buffer. 
  @param  Filename     Pointer to the source file name that generated the ASSERT().
  @param  Description  Pointer to the description of the ASSERT().
  @param  LineNumber   Pointer to source line number that generated the ASSERT().

  @retval  TRUE   The status code specified by CodeType, Value, and Data was 
                  converted ASSERT() arguments specified by Filename, Description, 
                  and LineNumber.
  @retval  FALSE  The status code specified by CodeType, Value, and Data could 
                  not be converted to ASSERT() arguments.

**/
BOOLEAN
EFIAPI
GlueReportStatusCodeExtractAssertInfo (
  IN EFI_STATUS_CODE_TYPE        CodeType,
  IN EFI_STATUS_CODE_VALUE       Value,  
  IN CONST EFI_STATUS_CODE_DATA  *Data, 
  OUT CHAR8                      **Filename,
  OUT CHAR8                      **Description,
  OUT UINT32                     *LineNumber
  );


/**
  Extracts DEBUG() information from a status code structure.

  Converts the status code specified by Data to the DEBUG() arguments specified 
  by ErrorLevel, Marker, and Format.  If type GUID in Data is 
  EFI_STATUS_CODE_DATA_TYPE_DEBUG_GUID, then extract ErrorLevel, Marker, and 
  Format from the optional data area of the status code buffer specified by Data.  
  The optional data area of Data contains a 32-bit ErrorLevel followed by Marker 
  which is 12 UINTN parameters, followed by a Null-terminated ASCII string for 
  the Format.  If the DEBUG() information could be extracted from Data, then 
  return TRUE.  Otherwise, FALSE is returned.

  If Data is NULL, then ASSERT().
  If ErrorLevel is NULL, then ASSERT().
  If Marker is NULL, then ASSERT().
  If Format is NULL, then ASSERT().

  @param  Data        Pointer to status code data buffer. 
  @param  ErrorLevel  Pointer to error level mask for a debug message.
  @param  Marker      Pointer to the variable argument list associated with Format.
  @param  Format      Pointer to a Null-terminated ASCII format string of a 
                      debug message.

  @retval  TRUE   The status code specified by Data was converted DEBUG() arguments 
                  specified by ErrorLevel, Marker, and Format.
  @retval  FALSE  The status code specified by Data could not be converted to 
                  DEBUG() arguments.

**/
BOOLEAN
EFIAPI
GlueReportStatusCodeExtractDebugInfo (
  IN CONST EFI_STATUS_CODE_DATA  *Data, 
  OUT UINT32                     *ErrorLevel,
  OUT VA_LIST                    *Marker,
  OUT CHAR8                      **Format
  );


/**
  Reports a status code.

  Reports the status code specified by the parameters Type and Value.  Status 
  code also require an instance, caller ID, and extended data.  This function 
  passed in a zero instance, NULL extended data, and a caller ID of 
  gEfiCallerIdGuid, which is the GUID for the module.  
  
  ReportStatusCode()must actively prevent recusrsion.  If ReportStatusCode() 
  is called while processing another any other Report Status Code Library function,
  then ReportStatusCode() must return immediately.

  @param  Type   Status code type. 
  @param  Value  Status code value.

  @retval  EFI_SUCCESS       The status code was reported.
  @retval  EFI_DEVICE_ERROR  There status code could not be reported due to a 
                             device error.
  @retval  EFI_UNSUPPORTED   Report status code is not supported

**/
EFI_STATUS
EFIAPI
GlueReportStatusCode (
  IN EFI_STATUS_CODE_TYPE   Type,
  IN EFI_STATUS_CODE_VALUE  Value
  );


/**
  Reports a status code with a Device Path Protocol as the extended data.

  Allocates and fills in the extended data section of a status code with the 
  Device Path Protocol specified by DevicePath.  This function is responsible 
  for allocating a buffer large enough for the standard header and the device 
  path.  The standard header is filled in with a GUID of 
  gEfiStatusCodeSpecificDataGuid.  The status code is reported with a zero 
  instance and a caller ID of gEfiCallerIdGuid.

  ReportStatusCodeWithDevicePath()must actively prevent recursion.  If 
  ReportStatusCodeWithDevicePath() is called while processing another any other 
  Report Status Code Library function, then ReportStatusCodeWithDevicePath() 
  must return EFI_DEVICE_ERROR immediately.

  If DevicePath is NULL, then ASSERT().

  @param  Type        Status code type. 
  @param  Value       Status code value.
  @param  DevicePath  Pointer to the Device Path Protocol to be reported.

  @retval  EFI_SUCCESS           The status code was reported with the extended 
                                 data specified by DevicePath.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate the 
                                 extended data section.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
EFI_STATUS
EFIAPI
GlueReportStatusCodeWithDevicePath (
  IN EFI_STATUS_CODE_TYPE            Type,
  IN EFI_STATUS_CODE_VALUE           Value,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  );


/**
  Reports a status code with an extended data buffer.

  Allocates and fills in the extended data section of a status code with the 
  extended data specified by ExtendedData and ExtendedDataSize.  ExtendedData 
  is assumed to be one of the data structures specified in Related Definitions.  
  These data structure do not have the standard header, so this function is 
  responsible for allocating a buffer large enough for the standard header and 
  the extended data passed into this function.  The standard header is filled 
  in with a GUID of  gEfiStatusCodeSpecificDataGuid.  The status code is reported 
  with a zero instance and a caller ID of gEfiCallerIdGuid.

  ReportStatusCodeWithExtendedData()must actively prevent recursion.  If 
  ReportStatusCodeWithExtendedData() is called while processing another any other 
  Report Status Code Library function, then ReportStatusCodeWithExtendedData() 
  must return EFI_DEVICE_ERROR immediately.

  If ExtendedData is NULL, then ASSERT().
  If ExtendedDataSize is 0, then ASSERT().

  @param  Type              Status code type. 
  @param  Value             Status code value.
  @param  ExtendedData      Pointer to the extended data buffer to be reported.
  @param  ExtendedDataSize  The size, in bytes, of the extended data buffer to 
                            be reported.

  @retval  EFI_SUCCESS           The status code was reported with the extended 
                                 data specified by ExtendedData and ExtendedDataSize.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate the 
                                 extended data section.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
EFI_STATUS
EFIAPI
GlueReportStatusCodeWithExtendedData (
  IN EFI_STATUS_CODE_TYPE   Type,
  IN EFI_STATUS_CODE_VALUE  Value,
  IN CONST VOID             *ExtendedData,
  IN UINTN                  ExtendedDataSize
  );


/**
  Reports a status code with full parameters.

  The function reports a status code.  If ExtendedData is NULL and ExtendedDataSize 
  is 0, then an extended data buffer is not reported.  If ExtendedData is not 
  NULL and ExtendedDataSize is not 0, then an extended data buffer is allocated.  
  ExtendedData is assumed not have the standard status code header, so this function 
  is responsible for allocating a buffer large enough for the standard header and 
  the extended data passed into this function.  The standard header is filled in 
  with a GUID specified by ExtendedDataGuid.  If ExtendedDataGuid is NULL, then a 
  GUID of gEfiStatusCodeSpecificDatauid is used.  The status code is reported with 
  an instance specified by Instance and a caller ID specified by CallerId.  If 
  CallerId is NULL, then a caller ID of gEfiCallerIdGuid is used.

  ReportStatusCodeEx()must actively prevent recursion.  If ReportStatusCodeEx() 
  is called while processing another any other Report Status Code Library function, 
  then ReportStatusCodeEx() must return EFI_DEVICE_ERROR immediately.

  If ExtendedData is NULL and ExtendedDataSize is not zero, then ASSERT().
  If ExtendedData is not NULL and ExtendedDataSize is zero, then ASSERT().

  @param  Type              Status code type. 
  @param  Value             Status code value.
  @param  Instance          Status code instance number.
  @param  CallerId          Pointer to a GUID that identifies the caller of this 
                            function.  If this parameter is NULL, then a caller 
                            ID of gEfiCallerIdGuid is used.
  @param  ExtendedDataGuid  Pointer to the GUID for the extended data buffer.  
                            If this parameter is NULL, then a the status code 
                            standard header is filled in with 
                            gEfiStatusCodeSpecificDataGuid.
  @param  ExtendedData      Pointer to the extended data buffer.  This is an 
                            optional parameter that may be NULL.
  @param  ExtendedDataSize  The size, in bytes, of the extended data buffer.

  @retval  EFI_SUCCESS           The status code was reported.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate 
                                 the extended data section if it was specified.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
EFI_STATUS
EFIAPI
GlueReportStatusCodeEx (
  IN EFI_STATUS_CODE_TYPE   Type,
  IN EFI_STATUS_CODE_VALUE  Value,
  IN UINT32                 Instance,
  IN CONST EFI_GUID         *CallerId          OPTIONAL,
  IN CONST EFI_GUID         *ExtendedDataGuid  OPTIONAL,
  IN CONST VOID             *ExtendedData      OPTIONAL,
  IN UINTN                  ExtendedDataSize
  );


/**
  Returns TRUE if status codes of type EFI_PROGRESS_CODE are enabled

  This function returns TRUE if the REPORT_STATUS_CODE_PROPERTY_PROGRESS_CODE_ENABLED 
  bit of PcdReportStatusCodeProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The REPORT_STATUS_CODE_PROPERTY_PROGRESS_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is set.
  @retval  FALSE  The REPORT_STATUS_CODE_PROPERTY_PROGRESS_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is clear.

**/
BOOLEAN
EFIAPI
GlueReportProgressCodeEnabled (
  VOID
  );


/**
  Returns TRUE if status codes of type EFI_ERROR_CODE are enabled

  This function returns TRUE if the REPORT_STATUS_CODE_PROPERTY_ERROR_CODE_ENABLED 
  bit of PcdReportStatusCodeProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The REPORT_STATUS_CODE_PROPERTY_ERROR_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is set.
  @retval  FALSE  The REPORT_STATUS_CODE_PROPERTY_ERROR_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is clear.

**/
BOOLEAN
EFIAPI
GlueReportErrorCodeEnabled (
  VOID
  );


/**
  Returns TRUE if status codes of type EFI_DEBUG_CODE are enabled

  This function returns TRUE if the REPORT_STATUS_CODE_PROPERTY_DEBUG_CODE_ENABLED 
  bit of PcdReportStatusCodeProperyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The REPORT_STATUS_CODE_PROPERTY_DEBUG_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is set.
  @retval  FALSE  The REPORT_STATUS_CODE_PROPERTY_DEBUG_CODE_ENABLED bit of 
                  PcdReportStatusCodeProperyMask is clear.

**/
BOOLEAN
EFIAPI
GlueReportDebugCodeEnabled (
  VOID
  );


/**
  Reports a status code with minimal parameters if the status code type is enabled.

  If the status code type specified by Type is enabled in 
  PcdReportStatusCodeProperyMask, then call ReportStatusCode() passing in Type 
  and Value.

  @param  Type   Status code type. 
  @param  Value  Status code value.

  @retval  EFI_SUCCESS       The status code was reported.
  @retval  EFI_DEVICE_ERROR  There status code could not be reported due to a device error.
  @retval  EFI_UNSUPPORTED   Report status code is not supported

**/
#define REPORT_STATUS_CODE(Type,Value)                                                          \
  (ReportProgressCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) ?  \
  ReportStatusCode(Type,Value)                                                               :  \
  (ReportErrorCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE)       ?  \
  ReportStatusCode(Type,Value)                                                               :  \
  (ReportDebugCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_DEBUG_CODE)       ?  \
  ReportStatusCode(Type,Value)                                                               :  \
  EFI_UNSUPPORTED


/**
  Reports a status code with a Device Path Protocol as the extended data if the 
  status code type is enabled.

  If the status code type specified by Type is enabled in 
  PcdReportStatusCodeProperyMask, then call ReportStatusCodeWithDevicePath() 
  passing in Type, Value, and DevicePath.

  @param  Type        Status code type. 
  @param  Value       Status code value.
  @param  DevicePath  Pointer to the Device Path Protocol to be reported.

  @retval  EFI_SUCCESS           The status code was reported with the extended 
                                 data specified by DevicePath.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate the 
                                 extended data section.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
#define REPORT_STATUS_CODE_WITH_DEVICE_PATH(Type,Value,DevicePathParameter)                     \
  (ReportProgressCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) ?  \
  ReportStatusCodeWithDevicePath(Type,Value,DevicePathParameter)                             :  \
  (ReportErrorCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE)       ?  \
  ReportStatusCodeWithDevicePath(Type,Value,DevicePathParameter)                             :  \
  (ReportDebugCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_DEBUG_CODE)       ?  \
  ReportStatusCodeWithDevicePath(Type,Value,DevicePathParameter)                             :  \
  EFI_UNSUPPORTED


/**
  Reports a status code with an extended data buffer if the status code type 
  is enabled.

  If the status code type specified by Type is enabled in 
  PcdReportStatusCodeProperyMask, then call ReportStatusCodeWithExtendedData() 
  passing in Type, Value, ExtendedData, and ExtendedDataSize.

  @param  Type              Status code type. 
  @param  Value             Status code value.
  @param  ExtendedData      Pointer to the extended data buffer to be reported.
  @param  ExtendedDataSize  The size, in bytes, of the extended data buffer to
                            be reported.

  @retval  EFI_SUCCESS           The status code was reported with the extended 
                                 data specified by ExtendedData and ExtendedDataSize.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate the 
                                 extended data section.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
#define REPORT_STATUS_CODE_WITH_EXTENDED_DATA(Type,Value,ExtendedData,ExtendedDataSize)         \
  (ReportProgressCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) ?  \
  ReportStatusCodeWithExtendedData(Type,Value,ExtendedData,ExtendedDataSize)                 :  \
  (ReportErrorCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE)       ?  \
  ReportStatusCodeWithExtendedData(Type,Value,ExtendedData,ExtendedDataSize)                 :  \
  (ReportDebugCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_DEBUG_CODE)       ?  \
  ReportStatusCodeWithExtendedData(Type,Value,ExtendedData,ExtendedDataSize)                 :  \
  EFI_UNSUPPORTED

/**
  Reports a status code specifying all parameters if the status code type is enabled.

  If the status code type specified by Type is enabled in 
  PcdReportStatusCodeProperyMask, then call ReportStatusCodeEx() passing in Type, 
  Value, Instance, CallerId, ExtendedDataGuid, ExtendedData, and ExtendedDataSize.

  @param  Type              Status code type. 
  @param  Value             Status code value.
  @param  Instance          Status code instance number.
  @param  CallerId          Pointer to a GUID that identifies the caller of this 
                            function.  If this parameter is NULL, then a caller 
                            ID of gEfiCallerIdGuid is used.
  @param  ExtendedDataGuid  Pointer to the GUID for the extended data buffer.  
                            If this parameter is NULL, then a the status code 
                            standard header is filled in with 
                            gEfiStatusCodeSpecificDataGuid.
  @param  ExtendedData      Pointer to the extended data buffer.  This is an 
                            optional parameter that may be NULL.
  @param  ExtendedDataSize  The size, in bytes, of the extended data buffer.

  @retval  EFI_SUCCESS           The status code was reported.
  @retval  EFI_OUT_OF_RESOURCES  There were not enough resources to allocate the 
                                 extended data section if it was specified.
  @retval  EFI_UNSUPPORTED       Report status code is not supported

**/
#define REPORT_STATUS_CODE_EX(Type,Value,Instance,CallerId,ExtendedDataGuid,ExtendedData,ExtendedDataSize)  \
  (ReportProgressCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE)             ?  \
  ReportStatusCodeEx(Type,Value,Instance,CallerId,ExtendedDataGuid,ExtendedData,ExtendedDataSize)        :  \
  (ReportErrorCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE)                   ?  \
  ReportStatusCodeEx(Type,Value,Instance,CallerId,ExtendedDataGuid,ExtendedData,ExtendedDataSize)        :  \
  (ReportDebugCodeEnabled() && ((Type) & EFI_STATUS_CODE_TYPE_MASK) == EFI_DEBUG_CODE)                   ?  \
  ReportStatusCodeEx(Type,Value,Instance,CallerId,ExtendedDataGuid,ExtendedData,ExtendedDataSize)        :  \
  EFI_UNSUPPORTED

#endif
