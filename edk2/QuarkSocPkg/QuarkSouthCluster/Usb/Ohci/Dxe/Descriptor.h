/** @file
This file contains the descriptor definination of OHCI spec

Copyright (c) 2013-2015 Intel Corporation.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/



#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#define ED_FUNC_ADD     0x0001
#define ED_ENDPT_NUM    0x0002
#define ED_DIR          0x0004
#define ED_SPEED        0x0008
#define ED_SKIP         0x0010
#define ED_FORMAT       0x0020
#define ED_MAX_PACKET   0x0040
#define ED_TDTAIL_PTR   0x0080
#define ED_HALTED       0x0100
#define ED_DTTOGGLE     0x0200
#define ED_TDHEAD_PTR   0x0400
#define ED_NEXT_EDPTR   0x0800
#define ED_PDATA        0x1000
#define ED_ZERO         0x2000

#define TD_BUFFER_ROUND     0x0001
#define TD_DIR_PID          0x0002
#define TD_DELAY_INT        0x0004
#define TD_DT_TOGGLE        0x0008
#define TD_ERROR_CNT        0x0010
#define TD_COND_CODE        0x0020
#define TD_CURR_BUFFER_PTR  0x0040
#define TD_NEXT_PTR         0x0080
#define TD_BUFFER_END_PTR   0x0100
#define TD_PDATA            0x0200

#define ED_FROM_TD_DIR        0x0
#define ED_OUT_DIR            0x1
#define ED_IN_DIR             0x2
#define ED_FROM_TD_ALSO_DIR   0x3

#define TD_SETUP_PID          0x00
#define TD_OUT_PID            0x01
#define TD_IN_PID             0x02
#define TD_NODATA_PID         0x03

#define HI_SPEED              0
#define LO_SPEED              1

#define TD_NO_ERROR           0x00
#define TD_CRC_ERROR          0x01
#define TD_BITSTUFFING_ERROR  0x02
#define TD_TOGGLE_ERROR       0x03
#define TD_DEVICE_STALL       0x04
#define TD_NO_RESPONSE        0x05
#define TD_PIDCHK_FAIL        0x06
#define TD_PID_UNEXPECTED     0x07
#define TD_DATA_OVERRUN       0x08
#define TD_DATA_UNDERRUN      0x09
#define TD_BUFFER_OVERRUN     0x0C
#define TD_BUFFER_UNDERRUN    0x0D
#define TD_TOBE_PROCESSED     0x0E
#define TD_TOBE_PROCESSED_2   0x0F

#define TD_NO_DELAY           0x7

#define TD_INT                0x1
#define TD_CTL                0x2
#define TD_BLK                0x3

typedef struct {
  UINT32 Reserved:18;
  UINT32 BufferRounding:1;
  UINT32 DirPID:2;
  UINT32 DelayInterrupt:3;
  UINT32 DataToggle:2;
  UINT32 ErrorCount:2;
  UINT32 ConditionCode:4;
} TD_DESCRIPTOR_WORD0;

typedef struct _TD_DESCRIPTOR {
  TD_DESCRIPTOR_WORD0     Word0;
  UINT32                  CurrBufferPointer;          // 32-bit Physical Address of buffer
  UINT32                  NextTD;                     // 32-bit Physical Address of TD_DESCRIPTOR
  UINT32                  BufferEndPointer;           // 32-bit Physical Address of buffer
  UINT32                  NextTDPointer;              // 32-bit Physical Address of TD_DESCRIPTOR
  UINT32                  DataBuffer;                 // 32-bit Physical Address of buffer
  UINT32                  ActualSendLength;
  UINT32                  Reserved;
} TD_DESCRIPTOR;

typedef struct {
  UINT32 FunctionAddress:7;
  UINT32 EndPointNum:4;
  UINT32 Direction:2;
  UINT32 Speed:1;
  UINT32 Skip:1;
  UINT32 Format:1;
  UINT32 MaxPacketSize:11;
  UINT32 FreeSpace:5;
} ED_DESCRIPTOR_WORD0;

typedef struct {
  UINT32 Halted:1;
  UINT32 ToggleCarry:1;
  UINT32 Zero:2;
  UINT32 TdHeadPointer:28;
} ED_DESCRIPTOR_WORD2;

typedef struct _ED_DESCRIPTOR {
  ED_DESCRIPTOR_WORD0     Word0;
  UINT32                  TdTailPointer;    // 32-bit Physical Address of TD_DESCRIPTOR
  ED_DESCRIPTOR_WORD2     Word2;
  UINT32                  NextED;           // 32-bit Physical Address of ED_DESCRIPTOR
} ED_DESCRIPTOR;

#define TD_PTR(p)            ((TD_DESCRIPTOR *)(UINTN)((p) << 4))
#define ED_PTR(p)            ((ED_DESCRIPTOR *)(UINTN)((p) << 4))
#define RIGHT_SHIFT_4(p)     ((UINT32)(p) >> 4)

typedef enum {
  CONTROL_LIST,
  BULK_LIST,
  INTERRUPT_LIST,
  ISOCHRONOUS_LIST
} DESCRIPTOR_LIST_TYPE;

#endif
