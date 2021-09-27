/**@file

Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:
  WinNtLib.h

Abstract:
  Public include file for the WinNt Library

**/

#ifndef __WIN_NT_DXE_H__
#define __WIN_NT_DXE_H__

//
// This forces Windows.h WIN32 include file to be included
//  it's needed for WinNtThunk.h
//  WinNtIo.h depends on WinNtThunk.h
//
#include <Common/WinNtInclude.h>
#endif
