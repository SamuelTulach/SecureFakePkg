/*++

Copyright (c) 2004 - 2011, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  EfiStatusCode.h

Abstract:

  Status Code Definitions, according to Intel Platform Innovation Framework 
  for EFI Status Codes Specification
  Revision 0.92

  The file is divided into sections for ease of use.

  Section:    Contents:
    1           General Status Code Definitions
    2           Class definitions
    3           Computing Unit Subclasses, Progress and Error Codes
    4           Peripheral Subclasses, Progress and Error Codes.
    5           IO Bus Subclasses, Progress and Error Codes.
    6           Software Subclasses, Progress and Error Codes.
    7           Debug Codes

--*/

#ifndef _EFI_STATUS_CODE_H_
#define _EFI_STATUS_CODE_H_

//
// /////////////////////////////////////////////////////////////////////////////
// Section 1
///////////////////////////////////////////////////////////////////////////////
//
// Status Code Type Definition
//
typedef UINT32  EFI_STATUS_CODE_TYPE;

//
// A Status Code Type is made up of the code type and severity
// All values masked by EFI_STATUS_CODE_RESERVED_MASK are
// reserved for use by this specification.
//
#define EFI_STATUS_CODE_TYPE_MASK     0x000000FF
#define EFI_STATUS_CODE_SEVERITY_MASK 0xFF000000
#define EFI_STATUS_CODE_RESERVED_MASK 0x00FFFF00

//
// Definition of code types, all other values masked by
// EFI_STATUS_CODE_TYPE_MASK are reserved for use by
// this specification.
//
#define EFI_PROGRESS_CODE 0x00000001
#define EFI_ERROR_CODE    0x00000002
#define EFI_DEBUG_CODE    0x00000003

//
// Definitions of severities, all other values masked by
// EFI_STATUS_CODE_SEVERITY_MASK are reserved for use by
// this specification.
//
#define EFI_ERROR_MINOR       0x40000000
#define EFI_ERROR_MAJOR       0x80000000
#define EFI_ERROR_UNRECOVERED 0x90000000
#define EFI_ERROR_UNCONTAINED 0xA0000000

//
// Status Code Value Definition
//
typedef UINT32  EFI_STATUS_CODE_VALUE;

//
// A Status Code Value is made up of the class, subclass, and
// an operation.  Classes, subclasses, and operations are defined
// in the following sections.
//
#define EFI_STATUS_CODE_CLASS_MASK      0xFF000000
#define EFI_STATUS_CODE_SUBCLASS_MASK   0x00FF0000
#define EFI_STATUS_CODE_OPERATION_MASK  0x0000FFFF

typedef struct {
  UINT16    HeaderSize;
  UINT16    Size;
  EFI_GUID  Type;
} EFI_STATUS_CODE_DATA;

//
// Data Hub Status Code class record definition
//
typedef struct {
  EFI_STATUS_CODE_TYPE  CodeType;
  EFI_STATUS_CODE_VALUE Value;
  UINT32                Instance;
  EFI_GUID              CallerId;
  EFI_STATUS_CODE_DATA  Data;
} DATA_HUB_STATUS_CODE_DATA_RECORD;

//
// /////////////////////////////////////////////////////////////////////////////
// Section 2
///////////////////////////////////////////////////////////////////////////////
//
// Class definitions
// Values of 4-127 are reserved for future use by this
// specification.
// Values in the range 128-255 are reserved for OEM use.
//
#define EFI_COMPUTING_UNIT  0x00000000
#define EFI_PERIPHERAL      0x01000000
#define EFI_IO_BUS          0x02000000
#define EFI_SOFTWARE        0x03000000

//
// General partitioning scheme for Progress and Error Codes are
// 0x0000-0x0FFF  - Shared by all sub-classes in a given class
// 0x1000-0x7FFF  - Subclass Specific
// 0x8000-0xFFFF  - OEM specific
//
#define EFI_SUBCLASS_SPECIFIC 0x1000
#define EFI_OEM_SPECIFIC      0x8000

//
// /////////////////////////////////////////////////////////////////////////////
// Section 3
///////////////////////////////////////////////////////////////////////////////
//
// Computing Unit Subclass definitions.
// Values of 7-127 are reserved for future use by this
// specification.
// Values of 128-255 are reserved for OEM use.
//
#define EFI_COMPUTING_UNIT_UNSPECIFIED        (EFI_COMPUTING_UNIT | 0x00000000)
#define EFI_COMPUTING_UNIT_HOST_PROCESSOR     (EFI_COMPUTING_UNIT | 0x00010000)
#define EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR (EFI_COMPUTING_UNIT | 0x00020000)
#define EFI_COMPUTING_UNIT_IO_PROCESSOR       (EFI_COMPUTING_UNIT | 0x00030000)
#define EFI_COMPUTING_UNIT_CACHE              (EFI_COMPUTING_UNIT | 0x00040000)
#define EFI_COMPUTING_UNIT_MEMORY             (EFI_COMPUTING_UNIT | 0x00050000)
#define EFI_COMPUTING_UNIT_CHIPSET            (EFI_COMPUTING_UNIT | 0x00060000)

//
// Computing Unit Class Progress Code definitions.
// These are shared by all subclasses.
//
#define EFI_CU_PC_INIT_BEGIN  0x00000000
#define EFI_CU_PC_INIT_END    0x00000001

//
// Computing Unit Unspecified Subclass Progress Code definitions.
//
//
// Computing Unit Host Processor Subclass Progress Code definitions.
//
#define EFI_CU_HP_PC_POWER_ON_INIT          (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_HP_PC_CACHE_INIT             (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_HP_PC_RAM_INIT               (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_CU_HP_PC_MEMORY_CONTROLLER_INIT (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_HP_PC_IO_INIT                (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_HP_PC_BSP_SELECT             (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_HP_PC_BSP_RESELECT           (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_CU_HP_PC_AP_INIT                (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_CU_HP_PC_SMM_INIT               (EFI_SUBCLASS_SPECIFIC | 0x00000008)

//
// Computing Unit Firmware Processor Subclass Progress Code definitions.
//
//
// Computing Unit IO Processor Subclass Progress Code definitions.
//
//
// Computing Unit Cache Subclass Progress Code definitions.
//
#define EFI_CU_CACHE_PC_PRESENCE_DETECT (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_CACHE_PC_CONFIGURATION   (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Computing Unit Memory Subclass Progress Code definitions.
//
#define EFI_CU_MEMORY_PC_SPD_READ         (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_MEMORY_PC_PRESENCE_DETECT  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_MEMORY_PC_TIMING           (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_CU_MEMORY_PC_CONFIGURING      (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_MEMORY_PC_OPTIMIZING       (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_MEMORY_PC_INIT             (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_MEMORY_PC_TEST             (EFI_SUBCLASS_SPECIFIC | 0x00000006)

//
// Computing Unit Chipset Subclass Progress Code definitions.
//
//
// Computing Unit Class Error Code definitions.
// These are shared by all subclasses.
//
#define EFI_CU_EC_NON_SPECIFIC    0x00000000
#define EFI_CU_EC_DISABLED        0x00000001
#define EFI_CU_EC_NOT_SUPPORTED   0x00000002
#define EFI_CU_EC_NOT_DETECTED    0x00000003
#define EFI_CU_EC_NOT_CONFIGURED  0x00000004

//
// Computing Unit Unspecified Subclass Error Code definitions.
//
//
// Computing Unit Host Processor Subclass Error Code definitions.
//
#define EFI_CU_HP_EC_INVALID_TYPE         (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_HP_EC_INVALID_SPEED        (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_HP_EC_MISMATCH             (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_CU_HP_EC_TIMER_EXPIRED        (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_HP_EC_SELF_TEST            (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_HP_EC_INTERNAL             (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_HP_EC_THERMAL              (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_CU_HP_EC_LOW_VOLTAGE          (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_CU_HP_EC_HIGH_VOLTAGE         (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_CU_HP_EC_CACHE                (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_CU_HP_EC_MICROCODE_UPDATE     (EFI_SUBCLASS_SPECIFIC | 0x0000000A)
#define EFI_CU_HP_EC_CORRECTABLE          (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define EFI_CU_HP_EC_UNCORRECTABLE        (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define EFI_CU_HP_EC_NO_MICROCODE_UPDATE  (EFI_SUBCLASS_SPECIFIC | 0x0000000D)

//
// Computing Unit Firmware Processor Subclass Error Code definitions.
//
#define EFI_CU_FP_EC_HARD_FAIL  (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_FP_EC_SOFT_FAIL  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_FP_EC_COMM_ERROR (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// Computing Unit IO Processor Subclass Error Code definitions.
//
//
// Computing Unit Cache Subclass Error Code definitions.
//
#define EFI_CU_CACHE_EC_INVALID_TYPE  (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_CACHE_EC_INVALID_SPEED (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_CACHE_EC_INVALID_SIZE  (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_CU_CACHE_EC_MISMATCH      (EFI_SUBCLASS_SPECIFIC | 0x00000003)

//
// Computing Unit Memory Subclass Error Code definitions.
//
#define EFI_CU_MEMORY_EC_INVALID_TYPE   (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_CU_MEMORY_EC_INVALID_SPEED  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_CU_MEMORY_EC_CORRECTABLE    (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_CU_MEMORY_EC_UNCORRECTABLE  (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_MEMORY_EC_SPD_FAIL       (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_MEMORY_EC_INVALID_SIZE   (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_MEMORY_EC_MISMATCH       (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_CU_MEMORY_EC_S3_RESUME_FAIL (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_CU_MEMORY_EC_UPDATE_FAIL    (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_CU_MEMORY_EC_NONE_DETECTED  (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_CU_MEMORY_EC_NONE_USEFUL    (EFI_SUBCLASS_SPECIFIC | 0x0000000A)

//
// Computing Unit Chipset Subclass Error Code definitions.
//

///////////////////////////////////////////////////////////////////////////////
// Section 4
///////////////////////////////////////////////////////////////////////////////
//
// Peripheral Subclass definitions.
// Values of 13-127 are reserved for future use by this
// specification.
// Values of 128-255 are reserved for OEM use.
//
#define EFI_PERIPHERAL_UNSPECIFIED      (EFI_PERIPHERAL | 0x00000000)
#define EFI_PERIPHERAL_KEYBOARD         (EFI_PERIPHERAL | 0x00010000)
#define EFI_PERIPHERAL_MOUSE            (EFI_PERIPHERAL | 0x00020000)
#define EFI_PERIPHERAL_LOCAL_CONSOLE    (EFI_PERIPHERAL | 0x00030000)
#define EFI_PERIPHERAL_REMOTE_CONSOLE   (EFI_PERIPHERAL | 0x00040000)
#define EFI_PERIPHERAL_SERIAL_PORT      (EFI_PERIPHERAL | 0x00050000)
#define EFI_PERIPHERAL_PARALLEL_PORT    (EFI_PERIPHERAL | 0x00060000)
#define EFI_PERIPHERAL_FIXED_MEDIA      (EFI_PERIPHERAL | 0x00070000)
#define EFI_PERIPHERAL_REMOVABLE_MEDIA  (EFI_PERIPHERAL | 0x00080000)
#define EFI_PERIPHERAL_AUDIO_INPUT      (EFI_PERIPHERAL | 0x00090000)
#define EFI_PERIPHERAL_AUDIO_OUTPUT     (EFI_PERIPHERAL | 0x000A0000)
#define EFI_PERIPHERAL_LCD_DEVICE       (EFI_PERIPHERAL | 0x000B0000)
#define EFI_PERIPHERAL_NETWORK          (EFI_PERIPHERAL | 0x000C0000)

//
// Peripheral Class Progress Code definitions.
// These are shared by all subclasses.
//
#define EFI_P_PC_INIT             0x00000000
#define EFI_P_PC_RESET            0x00000001
#define EFI_P_PC_DISABLE          0x00000002
#define EFI_P_PC_PRESENCE_DETECT  0x00000003
#define EFI_P_PC_ENABLE           0x00000004
#define EFI_P_PC_RECONFIG         0x00000005
#define EFI_P_PC_DETECTED         0x00000006

//
// Peripheral Class Unspecified Subclass Progress Code definitions.
//
//
// Peripheral Class Keyboard Subclass Progress Code definitions.
//
#define EFI_P_KEYBOARD_PC_CLEAR_BUFFER  (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_P_KEYBOARD_PC_SELF_TEST     (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Peripheral Class Mouse Subclass Progress Code definitions.
//
#define EFI_P_MOUSE_PC_SELF_TEST  (EFI_SUBCLASS_SPECIFIC | 0x00000000)

//
// Peripheral Class Local Console Subclass Progress Code definitions.
//
//
// Peripheral Class Remote Console Subclass Progress Code definitions.
//
//
// Peripheral Class Serial Port Subclass Progress Code definitions.
//
#define EFI_P_SERIAL_PORT_PC_CLEAR_BUFFER (EFI_SUBCLASS_SPECIFIC | 0x00000000)

//
// Peripheral Class Parallel Port Subclass Progress Code definitions.
//
//
// Peripheral Class Fixed Media Subclass Progress Code definitions.
//
//
// Peripheral Class Removable Media Subclass Progress Code definitions.
//
//
// Peripheral Class Audio Input Subclass Progress Code definitions.
//
//
// Peripheral Class Audio Output Subclass Progress Code definitions.
//
//
// Peripheral Class LCD Device Subclass Progress Code definitions.
//
//
// Peripheral Class Network Subclass Progress Code definitions.
//
//
// Peripheral Class Error Code definitions.
// These are shared by all subclasses.
//
#define EFI_P_EC_NON_SPECIFIC       0x00000000
#define EFI_P_EC_DISABLED           0x00000001
#define EFI_P_EC_NOT_SUPPORTED      0x00000002
#define EFI_P_EC_NOT_DETECTED       0x00000003
#define EFI_P_EC_NOT_CONFIGURED     0x00000004
#define EFI_P_EC_INTERFACE_ERROR    0x00000005
#define EFI_P_EC_CONTROLLER_ERROR   0x00000006
#define EFI_P_EC_INPUT_ERROR        0x00000007
#define EFI_P_EC_OUTPUT_ERROR       0x00000008
#define EFI_P_EC_RESOURCE_CONFLICT  0x00000009

//
// Peripheral Class Unspecified Subclass Error Code definitions.
//
//
// Peripheral Class Keyboard Subclass Error Code definitions.
//
#define EFI_P_KEYBOARD_EC_LOCKED    (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_P_KEYBOARD_EC_STUCK_KEY (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Peripheral Class Mouse Subclass Error Code definitions.
//
#define EFI_P_MOUSE_EC_LOCKED (EFI_SUBCLASS_SPECIFIC | 0x00000000)

//
// Peripheral Class Local Console Subclass Error Code definitions.
//
//
// Peripheral Class Remote Console Subclass Error Code definitions.
//
//
// Peripheral Class Serial Port Subclass Error Code definitions.
//
//
// Peripheral Class Parallel Port Subclass Error Code definitions.
//
//
// Peripheral Class Fixed Media Subclass Error Code definitions.
//
//
// Peripheral Class Removable Media Subclass Error Code definitions.
//
//
// Peripheral Class Audio Input Subclass Error Code definitions.
//
//
// Peripheral Class Audio Output Subclass Error Code definitions.
//
//
// Peripheral Class LCD Device Subclass Error Code definitions.
//
//
// Peripheral Class Network Subclass Error Code definitions.
//

///////////////////////////////////////////////////////////////////////////////
// Section 5
///////////////////////////////////////////////////////////////////////////////
//
// IO Bus Subclass definitions.
// Values of 13-127 are reserved for future use by this
// specification.
// Values of 128-255 are reserved for OEM use.
//
#define EFI_IO_BUS_UNSPECIFIED  (EFI_IO_BUS | 0x00000000)
#define EFI_IO_BUS_PCI          (EFI_IO_BUS | 0x00010000)
#define EFI_IO_BUS_USB          (EFI_IO_BUS | 0x00020000)
#define EFI_IO_BUS_IBA          (EFI_IO_BUS | 0x00030000)
#define EFI_IO_BUS_AGP          (EFI_IO_BUS | 0x00040000)
#define EFI_IO_BUS_PC_CARD      (EFI_IO_BUS | 0x00050000)
#define EFI_IO_BUS_LPC          (EFI_IO_BUS | 0x00060000)
#define EFI_IO_BUS_SCSI         (EFI_IO_BUS | 0x00070000)
#define EFI_IO_BUS_ATA_ATAPI    (EFI_IO_BUS | 0x00080000)
#define EFI_IO_BUS_FC           (EFI_IO_BUS | 0x00090000)
#define EFI_IO_BUS_IP_NETWORK   (EFI_IO_BUS | 0x000A0000)
#define EFI_IO_BUS_SMBUS        (EFI_IO_BUS | 0x000B0000)
#define EFI_IO_BUS_I2C          (EFI_IO_BUS | 0x000C0000)

//
// IO Bus Class Progress Code definitions.
// These are shared by all subclasses.
//
#define EFI_IOB_PC_INIT     0x00000000
#define EFI_IOB_PC_RESET    0x00000001
#define EFI_IOB_PC_DISABLE  0x00000002
#define EFI_IOB_PC_DETECT   0x00000003
#define EFI_IOB_PC_ENABLE   0x00000004
#define EFI_IOB_PC_RECONFIG 0x00000005
#define EFI_IOB_PC_HOTPLUG  0x00000006

//
// IO Bus Class Unspecified Subclass Progress Code definitions.
//
//
// IO Bus Class PCI Subclass Progress Code definitions.
//
#define EFI_IOB_PCI_PC_BUS_ENUM   (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_IOB_PCI_PC_RES_ALLOC  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_IOB_PCI_PC_HPC_INIT   (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// IO Bus Class USB Subclass Progress Code definitions.
//
//
// IO Bus Class IBA Subclass Progress Code definitions.
//
//
// IO Bus Class AGP Subclass Progress Code definitions.
//
//
// IO Bus Class PC Card Subclass Progress Code definitions.
//
//
// IO Bus Class LPC Subclass Progress Code definitions.
//
//
// IO Bus Class SCSI Subclass Progress Code definitions.
//
//
// IO Bus Class ATA/ATAPI Subclass Progress Code definitions.
//
#define EFI_IOB_ATA_BUS_SMART_ENABLE          (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_IOB_ATA_BUS_SMART_DISABLE         (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_IOB_ATA_BUS_SMART_OVERTHRESHOLD   (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_IOB_ATA_BUS_SMART_UNDERTHRESHOLD  (EFI_SUBCLASS_SPECIFIC | 0x00000003)

//
// IO Bus Class FC Subclass Progress Code definitions.
//
//
// IO Bus Class IP Network Subclass Progress Code definitions.
//
//
// IO Bus Class SMBUS Subclass Progress Code definitions.
//
//
// IO Bus Class I2C Subclass Progress Code definitions.
//
//
// IO Bus Class Error Code definitions.
// These are shared by all subclasses.
//
#define EFI_IOB_EC_NON_SPECIFIC       0x00000000
#define EFI_IOB_EC_DISABLED           0x00000001
#define EFI_IOB_EC_NOT_SUPPORTED      0x00000002
#define EFI_IOB_EC_NOT_DETECTED       0x00000003
#define EFI_IOB_EC_NOT_CONFIGURED     0x00000004
#define EFI_IOB_EC_INTERFACE_ERROR    0x00000005
#define EFI_IOB_EC_CONTROLLER_ERROR   0x00000006
#define EFI_IOB_EC_READ_ERROR         0x00000007
#define EFI_IOB_EC_WRITE_ERROR        0x00000008
#define EFI_IOB_EC_RESOURCE_CONFLICT  0x00000009

//
// IO Bus Class Unspecified Subclass Error Code definitions.
//
//
// IO Bus Class PCI Subclass Error Code definitions.
//
#define EFI_IOB_PCI_EC_PERR (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_IOB_PCI_EC_SERR (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// IO Bus Class USB Subclass Error Code definitions.
//
//
// IO Bus Class IBA Subclass Error Code definitions.
//
//
// IO Bus Class AGP Subclass Error Code definitions.
//
//
// IO Bus Class PC Card Subclass Error Code definitions.
//
//
// IO Bus Class LPC Subclass Error Code definitions.
//
//
// IO Bus Class SCSI Subclass Error Code definitions.
//
//
// IO Bus Class ATA/ATAPI Subclass Error Code definitions.
//
#define EFI_IOB_ATA_BUS_SMART_NOTSUPPORTED  (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_IOB_ATA_BUS_SMART_DISABLED      (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// IO Bus Class FC Subclass Error Code definitions.
//
//
// IO Bus Class IP Network Subclass Error Code definitions.
//
//
// IO Bus Class SMBUS Subclass Error Code definitions.
//
//
// IO Bus Class I2C Subclass Error Code definitions.
//

///////////////////////////////////////////////////////////////////////////////
// Section 6
///////////////////////////////////////////////////////////////////////////////
//
// Software Subclass definitions.
// Values of 20-127 are reserved for future use by this
// specification.
// Values of 128-255 are reserved for OEM use.
//
#define EFI_SOFTWARE_UNSPECIFIED          (EFI_SOFTWARE | 0x00000000)
#define EFI_SOFTWARE_SEC                  (EFI_SOFTWARE | 0x00010000)
#define EFI_SOFTWARE_PEI_CORE             (EFI_SOFTWARE | 0x00020000)
#define EFI_SOFTWARE_PEI_MODULE           (EFI_SOFTWARE | 0x00030000)
#define EFI_SOFTWARE_DXE_CORE             (EFI_SOFTWARE | 0x00040000)
#define EFI_SOFTWARE_DXE_BS_DRIVER        (EFI_SOFTWARE | 0x00050000)
#define EFI_SOFTWARE_DXE_RT_DRIVER        (EFI_SOFTWARE | 0x00060000)
#define EFI_SOFTWARE_SMM_DRIVER           (EFI_SOFTWARE | 0x00070000)
#define EFI_SOFTWARE_EFI_APPLICATION      (EFI_SOFTWARE | 0x00080000)
#define EFI_SOFTWARE_EFI_OS_LOADER        (EFI_SOFTWARE | 0x00090000)
#define EFI_SOFTWARE_RT                   (EFI_SOFTWARE | 0x000A0000)
#define EFI_SOFTWARE_AL                   (EFI_SOFTWARE | 0x000B0000)
#define EFI_SOFTWARE_EBC_EXCEPTION        (EFI_SOFTWARE | 0x000C0000)
#define EFI_SOFTWARE_IA32_EXCEPTION       (EFI_SOFTWARE | 0x000D0000)
#define EFI_SOFTWARE_IPF_EXCEPTION        (EFI_SOFTWARE | 0x000E0000)
#define EFI_SOFTWARE_PEI_SERVICE          (EFI_SOFTWARE | 0x000F0000)
#define EFI_SOFTWARE_EFI_BOOT_SERVICE     (EFI_SOFTWARE | 0x00100000)
#define EFI_SOFTWARE_EFI_RUNTIME_SERVICE  (EFI_SOFTWARE | 0x00110000)
#define EFI_SOFTWARE_EFI_DXE_SERVICE      (EFI_SOFTWARE | 0x00120000)
#define EFI_SOFTWARE_X64_EXCEPTION        (EFI_SOFTWARE | 0x00130000)

//
// Software Class Progress Code definitions.
// These are shared by all subclasses.
//
#define EFI_SW_PC_INIT                0x00000000
#define EFI_SW_PC_LOAD                0x00000001
#define EFI_SW_PC_INIT_BEGIN          0x00000002
#define EFI_SW_PC_INIT_END            0x00000003
#define EFI_SW_PC_AUTHENTICATE_BEGIN  0x00000004
#define EFI_SW_PC_AUTHENTICATE_END    0x00000005
#define EFI_SW_PC_INPUT_WAIT          0x00000006
#define EFI_SW_PC_USER_SETUP          0x00000007

//
// Software Class Unspecified Subclass Progress Code definitions.
//
//
// Software Class SEC Subclass Progress Code definitions.
//
#define EFI_SW_SEC_PC_ENTRY_POINT     (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_SEC_PC_HANDOFF_TO_NEXT (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Software Class PEI Core Subclass Progress Code definitions.
//
#define EFI_SW_PEI_CORE_PC_ENTRY_POINT      (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_PEI_CORE_PC_HANDOFF_TO_NEXT  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_PEI_CORE_PC_RETURN_TO_LAST   (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// Software Class PEI Module Subclass Progress Code definitions.
//
#define EFI_SW_PEIM_PC_RECOVERY_BEGIN (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_PEIM_PC_CAPSULE_LOAD   (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_PEIM_PC_CAPSULE_START  (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_PEIM_PC_RECOVERY_USER  (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_PEIM_PC_RECOVERY_AUTO  (EFI_SUBCLASS_SPECIFIC | 0x00000004)

//
// Software Class DXE Core Subclass Progress Code definitions.
//
#define EFI_SW_DXE_CORE_PC_ENTRY_POINT      (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_DXE_CORE_PC_HANDOFF_TO_NEXT  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_DXE_CORE_PC_RETURN_TO_LAST   (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_DXE_CORE_PC_START_DRIVER     (EFI_SUBCLASS_SPECIFIC | 0x00000003)

//
// Software Class DXE BS Driver Subclass Progress Code definitions.
//
#define EFI_SW_DXE_BS_PC_LEGACY_OPROM_INIT            (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_DXE_BS_PC_READY_TO_BOOT_EVENT          (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_DXE_BS_PC_LEGACY_BOOT_EVENT            (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_DXE_BS_PC_EXIT_BOOT_SERVICES_EVENT     (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_DXE_BS_PC_VIRTUAL_ADDRESS_CHANGE_EVENT (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_DXE_BS_PC_BEGIN_CONNECTING_DRIVERS     (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_SW_DXE_BS_PC_VERIFYING_PASSWORD           (EFI_SUBCLASS_SPECIFIC | 0x00000006)

//
// Software Class DXE RT Driver Subclass Progress Code definitions.
//
#define EFI_SW_DXE_RT_PC_S0 (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_DXE_RT_PC_S1 (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_DXE_RT_PC_S2 (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_DXE_RT_PC_S3 (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_DXE_RT_PC_S4 (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_DXE_RT_PC_S5 (EFI_SUBCLASS_SPECIFIC | 0x00000005)

//
// Software Class SMM Driver Subclass Progress Code definitions.
//
//
// Software Class EFI Application Subclass Progress Code definitions.
//
//
// Software Class EFI OS Loader Subclass Progress Code definitions.
//
//
// Software Class EFI RT Subclass Progress Code definitions.
//
#define EFI_SW_RT_PC_ENTRY_POINT      (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_RT_PC_HANDOFF_TO_NEXT  (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_RT_PC_RETURN_TO_LAST   (EFI_SUBCLASS_SPECIFIC | 0x00000002)

//
// Software Class EFI AL Subclass Progress Code definitions.
//
#define EFI_SW_AL_PC_ENTRY_POINT    (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_AL_PC_RETURN_TO_LAST (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Software Class EBC Exception Subclass Progress Code definitions.
//
//
// Software Class IA32 Exception Subclass Progress Code definitions.
//
//
// Software Class X64 Exception Subclass Progress Code definitions.
//
//
// Software Class IPF Exception Subclass Progress Code definitions.
//
//
// Software Class PEI Services Subclass Progress Code definitions.
//
#define EFI_SW_PS_PC_INSTALL_PPI            (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_PS_PC_REINSTALL_PPI          (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_PS_PC_LOCATE_PPI             (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_PS_PC_NOTIFY_PPI             (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_PS_PC_GET_BOOT_MODE          (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_PS_PC_SET_BOOT_MODE          (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_SW_PS_PC_GET_HOB_LIST           (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_SW_PS_PC_CREATE_HOB             (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_SW_PS_PC_FFS_FIND_NEXT_VOLUME   (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_SW_PS_PC_FFS_FIND_NEXT_FILE     (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_SW_PS_PC_FFS_FIND_SECTION_DATA  (EFI_SUBCLASS_SPECIFIC | 0x0000000A)
#define EFI_SW_PS_PC_INSTALL_PEI_MEMORY     (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define EFI_SW_PS_PC_ALLOCATE_PAGES         (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define EFI_SW_PS_PC_ALLOCATE_POOL          (EFI_SUBCLASS_SPECIFIC | 0x0000000D)
#define EFI_SW_PS_PC_COPY_MEM               (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define EFI_SW_PS_PC_SET_MEM                (EFI_SUBCLASS_SPECIFIC | 0x0000000F)

//
// Software Class EFI Boot Services Subclass Progress Code definitions.
//
#define EFI_SW_BS_PC_RAISE_TPL                      (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_BS_PC_RESTORE_TPL                    (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_BS_PC_ALLOCATE_PAGES                 (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_BS_PC_FREE_PAGES                     (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_BS_PC_GET_MEMORY_MAP                 (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_BS_PC_ALLOCATE_POOL                  (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_SW_BS_PC_FREE_POOL                      (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_SW_BS_PC_CREATE_EVENT                   (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_SW_BS_PC_SET_TIMER                      (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_SW_BS_PC_WAIT_FOR_EVENT                 (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_SW_BS_PC_SIGNAL_EVENT                   (EFI_SUBCLASS_SPECIFIC | 0x0000000A)
#define EFI_SW_BS_PC_CLOSE_EVENT                    (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define EFI_SW_BS_PC_CHECK_EVENT                    (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define EFI_SW_BS_PC_INSTALL_PROTOCOL_INTERFACE     (EFI_SUBCLASS_SPECIFIC | 0x0000000D)
#define EFI_SW_BS_PC_REINSTALL_PROTOCOL_INTERFACE   (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define EFI_SW_BS_PC_UNINSTALL_PROTOCOL_INTERFACE   (EFI_SUBCLASS_SPECIFIC | 0x0000000F)
#define EFI_SW_BS_PC_HANDLE_PROTOCOL                (EFI_SUBCLASS_SPECIFIC | 0x00000010)
#define EFI_SW_BS_PC_PC_HANDLE_PROTOCOL             (EFI_SUBCLASS_SPECIFIC | 0x00000011)
#define EFI_SW_BS_PC_REGISTER_PROTOCOL_NOTIFY       (EFI_SUBCLASS_SPECIFIC | 0x00000012)
#define EFI_SW_BS_PC_LOCATE_HANDLE                  (EFI_SUBCLASS_SPECIFIC | 0x00000013)
#define EFI_SW_BS_PC_INSTALL_CONFIGURATION_TABLE    (EFI_SUBCLASS_SPECIFIC | 0x00000014)
#define EFI_SW_BS_PC_LOAD_IMAGE                     (EFI_SUBCLASS_SPECIFIC | 0x00000015)
#define EFI_SW_BS_PC_START_IMAGE                    (EFI_SUBCLASS_SPECIFIC | 0x00000016)
#define EFI_SW_BS_PC_EXIT                           (EFI_SUBCLASS_SPECIFIC | 0x00000017)
#define EFI_SW_BS_PC_UNLOAD_IMAGE                   (EFI_SUBCLASS_SPECIFIC | 0x00000018)
#define EFI_SW_BS_PC_EXIT_BOOT_SERVICES             (EFI_SUBCLASS_SPECIFIC | 0x00000019)
#define EFI_SW_BS_PC_GET_NEXT_MONOTONIC_COUNT       (EFI_SUBCLASS_SPECIFIC | 0x0000001A)
#define EFI_SW_BS_PC_STALL                          (EFI_SUBCLASS_SPECIFIC | 0x0000001B)
#define EFI_SW_BS_PC_SET_WATCHDOG_TIMER             (EFI_SUBCLASS_SPECIFIC | 0x0000001C)
#define EFI_SW_BS_PC_CONNECT_CONTROLLER             (EFI_SUBCLASS_SPECIFIC | 0x0000001D)
#define EFI_SW_BS_PC_DISCONNECT_CONTROLLER          (EFI_SUBCLASS_SPECIFIC | 0x0000001E)
#define EFI_SW_BS_PC_OPEN_PROTOCOL                  (EFI_SUBCLASS_SPECIFIC | 0x0000001F)
#define EFI_SW_BS_PC_CLOSE_PROTOCOL                 (EFI_SUBCLASS_SPECIFIC | 0x00000020)
#define EFI_SW_BS_PC_OPEN_PROTOCOL_INFORMATION      (EFI_SUBCLASS_SPECIFIC | 0x00000021)
#define EFI_SW_BS_PC_PROTOCOLS_PER_HANDLE           (EFI_SUBCLASS_SPECIFIC | 0x00000022)
#define EFI_SW_BS_PC_LOCATE_HANDLE_BUFFER           (EFI_SUBCLASS_SPECIFIC | 0x00000023)
#define EFI_SW_BS_PC_LOCATE_PROTOCOL                (EFI_SUBCLASS_SPECIFIC | 0x00000024)
#define EFI_SW_BS_PC_INSTALL_MULTIPLE_INTERFACES    (EFI_SUBCLASS_SPECIFIC | 0x00000025)
#define EFI_SW_BS_PC_UNINSTALL_MULTIPLE_INTERFACES  (EFI_SUBCLASS_SPECIFIC | 0x00000026)
#define EFI_SW_BS_PC_CALCULATE_CRC_32               (EFI_SUBCLASS_SPECIFIC | 0x00000027)
#define EFI_SW_BS_PC_COPY_MEM                       (EFI_SUBCLASS_SPECIFIC | 0x00000028)
#define EFI_SW_BS_PC_SET_MEM                        (EFI_SUBCLASS_SPECIFIC | 0x00000029)

//
// Software Class EFI Runtime Services Subclass Progress Code definitions.
//
#define EFI_SW_RS_PC_GET_TIME                       (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_RS_PC_SET_TIME                       (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_RS_PC_GET_WAKEUP_TIME                (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_RS_PC_SET_WAKEUP_TIME                (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_RS_PC_SET_VIRTUAL_ADDRESS_MAP        (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_RS_PC_CONVERT_POINTER                (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_SW_RS_PC_GET_VARIABLE                   (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_SW_RS_PC_GET_NEXT_VARIABLE_NAME         (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_SW_RS_PC_SET_VARIABLE                   (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_SW_RS_PC_GET_NEXT_HIGH_MONOTONIC_COUNT  (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_SW_RS_PC_RESET_SYSTEM                   (EFI_SUBCLASS_SPECIFIC | 0x0000000A)

//
// Software Class EFI DXE Services Subclass Progress Code definitions
//
#define EFI_SW_DS_PC_ADD_MEMORY_SPACE             (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_DS_PC_ALLOCATE_MEMORY_SPACE        (EFI_SUBCLASS_SPECIFIC | 0x00000001)
#define EFI_SW_DS_PC_FREE_MEMORY_SPACE            (EFI_SUBCLASS_SPECIFIC | 0x00000002)
#define EFI_SW_DS_PC_REMOVE_MEMORY_SPACE          (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_SW_DS_PC_GET_MEMORY_SPACE_DESCRIPTOR  (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_SW_DS_PC_SET_MEMORY_SPACE_ATTRIBUTES  (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_SW_DS_PC_GET_MEMORY_SPACE_MAP         (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_SW_DS_PC_ADD_IO_SPACE                 (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_SW_DS_PC_ALLOCATE_IO_SPACE            (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_SW_DS_PC_FREE_IO_SPACE                (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_SW_DS_PC_REMOVE_IO_SPACE              (EFI_SUBCLASS_SPECIFIC | 0x0000000A)
#define EFI_SW_DS_PC_GET_IO_SPACE_DESCRIPTOR      (EFI_SUBCLASS_SPECIFIC | 0x0000000B)
#define EFI_SW_DS_PC_GET_IO_SPACE_MAP             (EFI_SUBCLASS_SPECIFIC | 0x0000000C)
#define EFI_SW_DS_PC_DISPATCH                     (EFI_SUBCLASS_SPECIFIC | 0x0000000D)
#define EFI_SW_DS_PC_SCHEDULE                     (EFI_SUBCLASS_SPECIFIC | 0x0000000E)
#define EFI_SW_DS_PC_TRUST                        (EFI_SUBCLASS_SPECIFIC | 0x0000000F)
#define EFI_SW_DS_PC_PROCESS_FIRMWARE_VOLUME      (EFI_SUBCLASS_SPECIFIC | 0x00000010)

//
// Software Class Error Code definitions.
// These are shared by all subclasses.
//
#define EFI_SW_EC_NON_SPECIFIC            0x00000000
#define EFI_SW_EC_LOAD_ERROR              0x00000001
#define EFI_SW_EC_INVALID_PARAMETER       0x00000002
#define EFI_SW_EC_UNSUPPORTED             0x00000003
#define EFI_SW_EC_INVALID_BUFFER          0x00000004
#define EFI_SW_EC_OUT_OF_RESOURCES        0x00000005
#define EFI_SW_EC_ABORTED                 0x00000006
#define EFI_SW_EC_ILLEGAL_SOFTWARE_STATE  0x00000007
#define EFI_SW_EC_ILLEGAL_HARDWARE_STATE  0x00000008
#define EFI_SW_EC_START_ERROR             0x00000009
#define EFI_SW_EC_BAD_DATE_TIME           0x0000000A
#define EFI_SW_EC_CFG_INVALID             0x0000000B
#define EFI_SW_EC_CFG_CLR_REQUEST         0x0000000C
#define EFI_SW_EC_CFG_DEFAULT             0x0000000D
#define EFI_SW_EC_PWD_INVALID             0x0000000E
#define EFI_SW_EC_PWD_CLR_REQUEST         0x0000000F
#define EFI_SW_EC_PWD_CLEARED             0x00000010
#define EFI_SW_EC_EVENT_LOG_FULL          0x00000011

//
// Software Class Unspecified Subclass Error Code definitions.
//
//
// Software Class SEC Subclass Error Code definitions.
//
//
// Software Class PEI Core Subclass Error Code definitions.
//
#define EFI_SW_PEI_CORE_EC_DXE_CORRUPT  (EFI_SUBCLASS_SPECIFIC | 0x00000000)

//
// Software Class PEI Module Subclass Error Code definitions.
//
#define EFI_SW_PEIM_EC_NO_RECOVERY_CAPSULE        (EFI_SUBCLASS_SPECIFIC | 0x00000000)
#define EFI_SW_PEIM_EC_INVALID_CAPSULE_DESCRIPTOR (EFI_SUBCLASS_SPECIFIC | 0x00000001)

//
// Software Class DXE Core Subclass Error Code definitions.
//
#define EFI_SW_CSM_LEGACY_ROM_INIT  (EFI_SUBCLASS_SPECIFIC | 0x00000000)
//
// Software Class DXE Boot Service Driver Subclass Error Code definitions.
//
#define EFI_SW_DXE_BS_EC_LEGACY_OPROM_NO_SPACE  (EFI_SUBCLASS_SPECIFIC | 0x00000000)

//
// Software Class DXE Runtime Service Driver Subclass Error Code definitions.
//
//
// Software Class SMM Driver Subclass Error Code definitions.
//
//
// Software Class EFI Application Subclass Error Code definitions.
//
//
// Software Class EFI OS Loader Subclass Error Code definitions.
//
//
// Software Class EFI RT Subclass Error Code definitions.
//
//
// Software Class EFI AL Subclass Error Code definitions.
//
//
// Software Class EBC Exception Subclass Error Code definitions.
// These exceptions are derived from the debug protocol definitions in the EFI
// specification.
//
#define EFI_SW_EC_EBC_UNDEFINED             (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_UNDEFINED)
#define EFI_SW_EC_EBC_DIVIDE_ERROR          (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_DIVIDE_ERROR)
#define EFI_SW_EC_EBC_DEBUG                 (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_DEBUG)
#define EFI_SW_EC_EBC_BREAKPOINT            (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_BREAKPOINT)
#define EFI_SW_EC_EBC_OVERFLOW              (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_OVERFLOW)
#define EFI_SW_EC_EBC_INVALID_OPCODE        (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_INVALID_OPCODE)
#define EFI_SW_EC_EBC_STACK_FAULT           (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_STACK_FAULT)
#define EFI_SW_EC_EBC_ALIGNMENT_CHECK       (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_ALIGNMENT_CHECK)
#define EFI_SW_EC_EBC_INSTRUCTION_ENCODING  (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_INSTRUCTION_ENCODING)
#define EFI_SW_EC_EBC_BAD_BREAK             (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_BAD_BREAK)
#define EFI_SW_EC_EBC_STEP                  (EFI_SUBCLASS_SPECIFIC | EXCEPT_EBC_STEP)

//
// Software Class IA32 Exception Subclass Error Code definitions.
// These exceptions are derived from the debug protocol definitions in the EFI
// specification.
//
#define EFI_SW_EC_IA32_DIVIDE_ERROR     (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_DIVIDE_ERROR)
#define EFI_SW_EC_IA32_DEBUG            (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_DEBUG)
#define EFI_SW_EC_IA32_NMI              (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_NMI)
#define EFI_SW_EC_IA32_BREAKPOINT       (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_BREAKPOINT)
#define EFI_SW_EC_IA32_OVERFLOW         (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_OVERFLOW)
#define EFI_SW_EC_IA32_BOUND            (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_BOUND)
#define EFI_SW_EC_IA32_INVALID_OPCODE   (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_INVALID_OPCODE)
#define EFI_SW_EC_IA32_DOUBLE_FAULT     (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_DOUBLE_FAULT)
#define EFI_SW_EC_IA32_INVALID_TSS      (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_INVALID_TSS)
#define EFI_SW_EC_IA32_SEG_NOT_PRESENT  (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_SEG_NOT_PRESENT)
#define EFI_SW_EC_IA32_STACK_FAULT      (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_STACK_FAULT)
#define EFI_SW_EC_IA32_GP_FAULT         (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_GP_FAULT)
#define EFI_SW_EC_IA32_PAGE_FAULT       (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_PAGE_FAULT)
#define EFI_SW_EC_IA32_FP_ERROR         (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_FP_ERROR)
#define EFI_SW_EC_IA32_ALIGNMENT_CHECK  (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_ALIGNMENT_CHECK)
#define EFI_SW_EC_IA32_MACHINE_CHECK    (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_MACHINE_CHECK)
#define EFI_SW_EC_IA32_SIMD             (EFI_SUBCLASS_SPECIFIC | EXCEPT_IA32_SIMD)

//
// Software Class X64 Exception Subclass Error Code definitions.
// These exceptions are derived from the debug protocol definitions in the EFI
// specification.
//
#define EFI_SW_EC_X64_DIVIDE_ERROR      (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_DIVIDE_ERROR)
#define EFI_SW_EC_X64_DEBUG             (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_DEBUG)
#define EFI_SW_EC_X64_NMI               (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_NMI)
#define EFI_SW_EC_X64_BREAKPOINT        (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_BREAKPOINT)
#define EFI_SW_EC_X64_OVERFLOW          (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_OVERFLOW)
#define EFI_SW_EC_X64_BOUND             (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_BOUND)
#define EFI_SW_EC_X64_INVALID_OPCODE    (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_INVALID_OPCODE)
#define EFI_SW_EC_X64_DOUBLE_FAULT      (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_DOUBLE_FAULT)
#define EFI_SW_EC_X64_INVALID_TSS       (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_INVALID_TSS)
#define EFI_SW_EC_X64_SEG_NOT_PRESENT   (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_SEG_NOT_PRESENT)
#define EFI_SW_EC_X64_STACK_FAULT       (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_STACK_FAULT)
#define EFI_SW_EC_X64_GP_FAULT          (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_GP_FAULT)
#define EFI_SW_EC_X64_PAGE_FAULT        (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_PAGE_FAULT)
#define EFI_SW_EC_X64_FP_ERROR          (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_FP_ERROR)
#define EFI_SW_EC_X64_ALIGNMENT_CHECK   (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_ALIGNMENT_CHECK)
#define EFI_SW_EC_X64_MACHINE_CHECK     (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_MACHINE_CHECK)
#define EFI_SW_EC_X64_SIMD              (EFI_SUBCLASS_SPECIFIC | EXCEPT_X64_SIMD)

//
// Software Class IPF Exception Subclass Error Code definitions.
// These exceptions are derived from the debug protocol definitions in the EFI
// specification.
//
#define EFI_SW_EC_IPF_ALT_DTLB            (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_ALT_DTLB)
#define EFI_SW_EC_IPF_DNESTED_TLB         (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_DNESTED_TLB)
#define EFI_SW_EC_IPF_BREAKPOINT          (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_BREAKPOINT)
#define EFI_SW_EC_IPF_EXTERNAL_INTERRUPT  (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_EXTERNAL_INTERRUPT)
#define EFI_SW_EC_IPF_GEN_EXCEPT          (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_GEN_EXCEPT)
#define EFI_SW_EC_IPF_NAT_CONSUMPTION     (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_NAT_CONSUMPTION)
#define EFI_SW_EC_IPF_DEBUG_EXCEPT        (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_DEBUG_EXCEPT)
#define EFI_SW_EC_IPF_UNALIGNED_ACCESS    (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_UNALIGNED_ACCESS)
#define EFI_SW_EC_IPF_FP_FAULT            (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_FP_FAULT)
#define EFI_SW_EC_IPF_FP_TRAP             (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_FP_TRAP)
#define EFI_SW_EC_IPF_TAKEN_BRANCH        (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_TAKEN_BRANCH)
#define EFI_SW_EC_IPF_SINGLE_STEP         (EFI_SUBCLASS_SPECIFIC | EXCEPT_IPF_SINGLE_STEP)

//
// Software Class PEI Service Subclass Error Code definitions.
//
//
// Software Class EFI Boot Service Subclass Error Code definitions.
//
//
// Software Class EFI Runtime Service Subclass Error Code definitions.
//
//
// Software Class EFI DXE Service Subclass Error Code definitions.
//

///////////////////////////////////////////////////////////////////////////////
// Section 7
///////////////////////////////////////////////////////////////////////////////
//
// Debug Code definitions for all classes and subclass
// Only one debug code is defined at this point and should
// be used for anything that gets sent to debug stream.
//
#define EFI_DC_UNSPECIFIED  0x0

//
// Progress code for S3 suspend start and S3 suspend end is not defined,
// the following are the implementation specific definition.
//
#ifndef PROGRESS_CODE_S3_SUSPEND_START
#define PROGRESS_CODE_S3_SUSPEND_START   (EFI_SOFTWARE_SMM_DRIVER | (EFI_OEM_SPECIFIC | 0x00000000))
#endif

#ifndef PROGRESS_CODE_S3_SUSPEND_END
#define PROGRESS_CODE_S3_SUSPEND_END     (EFI_SOFTWARE_SMM_DRIVER | (EFI_OEM_SPECIFIC | 0x00000001))
#endif

#endif
