/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  TianoBind.h

Abstract:

  Tiano's Processor or Compiler specific defines and types for IA-32 
  besides EfiBind.h.

--*/

#ifndef _TIANO_BIND_H_
#define _TIANO_BIND_H_

#include <EfiBind.h>

#ifdef EFI_DEBUG

#ifdef EFI_NT_EMULATOR

#define EFI_DXE_ENTRY_POINT(InitFunction)                     \
					VOID                                                \
					EFIAPI                                              \
					InitFunction (                                      \
  					IN  VOID *HobStart																\
  				);																									\
																															\
          UINTN                                               \
          __stdcall                                           \
          _DllMainCRTStartup (                                \
              UINTN    Inst,                                  \
              UINTN    reason_for_call,                       \
              VOID    *rserved                                \
              )                                               \
          {                                                   \
              return 1;                                       \
          }                                                   \
                                                              \
          VOID                                                \
          __declspec( dllexport  )                            \
          __cdecl                                             \
          InitializeDriver (                                  \
              VOID *Hob                                       \
              )                                               \
          {                                                   \
              InitFunction(Hob);                              \
          }




#define EFI_SMI_HANDLER_ENTRY_POINT(InitFunction)             \
          UINTN                                               \
          __stdcall                                           \
          _DllMainCRTStartup (                                \
              UINTN    Inst,                                  \
              UINTN    reason_for_call,                       \
              VOID    *rserved                                \
              )                                               \
          {                                                   \
              return 1;                                       \
          }                                                   \
                                                              \
          EFI_STATUS                                                 \
          __declspec( dllexport  )                            \
          __cdecl                                             \
          InitializeDriver (                                  \
              VOID  *ImageHandle,                             \
              VOID  *Smst,                                    \
              VOID  *CommunicationBuffer,                     \
              UINTN *SourceSize                               \
              )                                               \
          {                                                   \
              return InitFunction(ImageHandle, Smst, CommunicationBuffer, SourceSize);  \
          }

#else

#define EFI_DXE_ENTRY_POINT(InitFunction)
#define EFI_SMI_HANDLER_ENTRY_POINT(InitFunction)

#endif

#else

#define EFI_DXE_ENTRY_POINT(InitFunction)
#define EFI_SMI_HANDLER_ENTRY_POINT(InitFunction)

#endif

#endif
