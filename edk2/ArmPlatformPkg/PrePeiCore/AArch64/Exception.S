#
#  Copyright (c) 2011-2014, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

#include <Chipset/AArch64.h>
#include <AsmMacroIoLibV8.h>
#include <Base.h>
#include <AutoGen.h>

.text

//============================================================
//Default Exception Handlers
//============================================================

#define TO_HANDLER                                              \
   EL1_OR_EL2(x1)                                               \
1: mrs  x1, elr_el1    /* EL1 Exception Link Register */       ;\
   b    3f                                                     ;\
2: mrs  x1, elr_el2    /* EL2 Exception Link Register */       ;\
3: bl   ASM_PFX(PeiCommonExceptionEntry)                       ;


//
// Default Exception handlers: There is no plan to return from any of these exceptions.
// No context saving at all.
//

VECTOR_BASE(PeiVectorTable)

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SP0_SYNC)
_DefaultSyncExceptHandler_t:
  mov  x0, #EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SP0_IRQ)
_DefaultIrq_t:
  mov  x0, #EXCEPT_AARCH64_IRQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SP0_FIQ)
_DefaultFiq_t:
  mov  x0, #EXCEPT_AARCH64_FIQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SP0_SERR)
_DefaultSError_t:
  mov  x0, #EXCEPT_AARCH64_SERROR
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SPx_SYNC)
_DefaultSyncExceptHandler_h:
  mov  x0, #EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SPx_IRQ)
_DefaultIrq_h:
  mov  x0, #EXCEPT_AARCH64_IRQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SPx_FIQ)
_DefaultFiq_h:
  mov  x0, #EXCEPT_AARCH64_FIQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_CUR_SPx_SERR)
_DefaultSError_h:
  mov  x0, #EXCEPT_AARCH64_SERROR
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A64_SYNC)
_DefaultSyncExceptHandler_LowerA64:
  mov  x0, #EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A64_IRQ)
_DefaultIrq_LowerA64:
  mov  x0, #EXCEPT_AARCH64_IRQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A64_FIQ)
_DefaultFiq_LowerA64:
  mov  x0, #EXCEPT_AARCH64_FIQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A64_SERR)
_DefaultSError_LowerA64:
  mov  x0, #EXCEPT_AARCH64_SERROR
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A32_SYNC)
_DefaultSyncExceptHandler_LowerA32:
  mov  x0, #EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A32_IRQ)
_DefaultIrq_LowerA32:
  mov  x0, #EXCEPT_AARCH64_IRQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A32_FIQ)
_DefaultFiq_LowerA32:
  mov  x0, #EXCEPT_AARCH64_FIQ
  TO_HANDLER

VECTOR_ENTRY(PeiVectorTable, ARM_VECTOR_LOW_A32_SERR)
_DefaultSError_LowerA32:
  mov  x0, #EXCEPT_AARCH64_SERROR
  TO_HANDLER

VECTOR_END(PeiVectorTable)
