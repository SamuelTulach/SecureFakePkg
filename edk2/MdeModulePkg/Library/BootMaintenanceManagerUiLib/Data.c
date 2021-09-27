/** @file
Define some data used for Boot Maint

Copyright (c) 2004 - 2015, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "BootMaintenanceManager.h"

VOID                *mStartOpCodeHandle = NULL;
VOID                *mEndOpCodeHandle = NULL;
EFI_IFR_GUID_LABEL  *mStartLabel = NULL;
EFI_IFR_GUID_LABEL  *mEndLabel = NULL;

///
/// Terminal type string token storage
///
UINT16              TerminalType[] = {
  STRING_TOKEN(STR_COM_TYPE_0),
  STRING_TOKEN(STR_COM_TYPE_1),
  STRING_TOKEN(STR_COM_TYPE_2),
  STRING_TOKEN(STR_COM_TYPE_3),
  STRING_TOKEN(STR_COM_TYPE_4),
};

///
/// Flow Control type string token storage
///
UINT16              mFlowControlType[2] = {
  STRING_TOKEN(STR_NONE_FLOW_CONTROL),
  STRING_TOKEN(STR_HARDWARE_FLOW_CONTROL)
};

UINT32              mFlowControlValue[2] = {
  0,
  UART_FLOW_CONTROL_HARDWARE
};

///
/// Console Input Device Selection Menu
///
BM_MENU_OPTION      ConsoleInpMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Console Output Device Selection Menu
///
BM_MENU_OPTION      ConsoleOutMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Error Output Device Selection Menu
///
BM_MENU_OPTION      ConsoleErrMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Boot Option from variable Menu
///
BM_MENU_OPTION      BootOptionMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Driver Option from variable menu
///
BM_MENU_OPTION      DriverOptionMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Handles in current system selection menu
///
BM_MENU_OPTION      DriverMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

BM_MENU_OPTION      TerminalMenu = {
  BM_MENU_OPTION_SIGNATURE,
  {NULL},
  0
};

///
/// Value and string token correspondency for BaudRate
///
COM_ATTR            BaudRateList[19] = {
  {
    115200,
    STRING_TOKEN(STR_COM_BAUD_RATE_0)
  },
  {
    57600,
    STRING_TOKEN(STR_COM_BAUD_RATE_1)
  },
  {
    38400,
    STRING_TOKEN(STR_COM_BAUD_RATE_2)
  },
  {
    19200,
    STRING_TOKEN(STR_COM_BAUD_RATE_3)
  },
  {
    9600,
    STRING_TOKEN(STR_COM_BAUD_RATE_4)
  },
  {
    7200,
    STRING_TOKEN(STR_COM_BAUD_RATE_5)
  },
  {
    4800,
    STRING_TOKEN(STR_COM_BAUD_RATE_6)
  },
  {
    3600,
    STRING_TOKEN(STR_COM_BAUD_RATE_7)
  },
  {
    2400,
    STRING_TOKEN(STR_COM_BAUD_RATE_8)
  },
  {
    2000,
    STRING_TOKEN(STR_COM_BAUD_RATE_9)
  },
  {
    1800,
    STRING_TOKEN(STR_COM_BAUD_RATE_10)
  },
  {
    1200,
    STRING_TOKEN(STR_COM_BAUD_RATE_11)
  },
  {
    600,
    STRING_TOKEN(STR_COM_BAUD_RATE_12)
  },
  {
    300,
    STRING_TOKEN(STR_COM_BAUD_RATE_13)
  },
  {
    150,
    STRING_TOKEN(STR_COM_BAUD_RATE_14)
  },
  {
    134,
    STRING_TOKEN(STR_COM_BAUD_RATE_15)
  },
  {
    110,
    STRING_TOKEN(STR_COM_BAUD_RATE_16)
  },
  {
    75,
    STRING_TOKEN(STR_COM_BAUD_RATE_17)
  },
  {
    50,
    STRING_TOKEN(STR_COM_BAUD_RATE_18)
  }
};

///
/// Value and string token correspondency for DataBits
///
COM_ATTR            DataBitsList[4] = {
  {
    5,
    STRING_TOKEN(STR_COM_DATA_BITS_0)
  },
  {
    6,
    STRING_TOKEN(STR_COM_DATA_BITS_1)
  },
  {
    7,
    STRING_TOKEN(STR_COM_DATA_BITS_2)
  },
  {
    8,
    STRING_TOKEN(STR_COM_DATA_BITS_3)
  }
};

///
/// Value and string token correspondency for Parity
///
COM_ATTR            ParityList[5] = {
  {
    NoParity,
    STRING_TOKEN(STR_COM_PAR_0)
  },
  {
    EvenParity,
    STRING_TOKEN(STR_COM_PAR_1)
  },
  {
    OddParity,
    STRING_TOKEN(STR_COM_PAR_2)
  },
  {
    MarkParity,
    STRING_TOKEN(STR_COM_PAR_3)
  },
  {
    SpaceParity,
    STRING_TOKEN(STR_COM_PAR_4)
  }
};

///
/// Value and string token correspondency for Baudreate
///
COM_ATTR            StopBitsList[3] = {
  {
    OneStopBit,
    STRING_TOKEN(STR_COM_STOP_BITS_0)
  },
  {
    OneFiveStopBits,
    STRING_TOKEN(STR_COM_STOP_BITS_1)
  },
  {
    TwoStopBits,
    STRING_TOKEN(STR_COM_STOP_BITS_2)
  }
};

///
/// Guid for messaging path, used in Serial port setting.
///
EFI_GUID            TerminalTypeGuid[] = {
  DEVICE_PATH_MESSAGING_PC_ANSI,
  DEVICE_PATH_MESSAGING_VT_100,
  DEVICE_PATH_MESSAGING_VT_100_PLUS,
  DEVICE_PATH_MESSAGING_VT_UTF8,
  EFI_TTY_TERM_GUID
};
