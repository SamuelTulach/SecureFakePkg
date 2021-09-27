/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>
  Portions copyright (c) 2011, 2012, ARM Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SEMIHOSTING_H__
#define __SEMIHOSTING_H__

/*
 *
 *  Please refer to ARM RVDS 3.0 Compiler and Libraries Guide for more information
 *  about the semihosting interface.
 *
 */

#define SEMIHOST_FILE_MODE_READ     (0 << 2)
#define SEMIHOST_FILE_MODE_WRITE    (1 << 2)
#define SEMIHOST_FILE_MODE_APPEND   (2 << 2)
#define SEMIHOST_FILE_MODE_UPDATE   (1 << 1)
#define SEMIHOST_FILE_MODE_BINARY   (1 << 0)
#define SEMIHOST_FILE_MODE_ASCII    (0 << 0)

BOOLEAN
SemihostConnectionSupported (
  VOID
  );

RETURN_STATUS
SemihostFileOpen (
  IN  CHAR8  *FileName,
  IN  UINT32 Mode,
  OUT UINTN  *FileHandle
  );

RETURN_STATUS
SemihostFileSeek (
  IN UINTN  FileHandle,
  IN UINTN  Offset
  );

RETURN_STATUS
SemihostFileRead (
  IN     UINTN  FileHandle,
  IN OUT UINTN  *Length,
  OUT    VOID   *Buffer
  );

RETURN_STATUS
SemihostFileWrite (
  IN     UINTN  FileHandle,
  IN OUT UINTN  *Length,
  IN     VOID   *Buffer
  );

RETURN_STATUS
SemihostFileClose (
  IN UINTN  FileHandle
  );

RETURN_STATUS
SemihostFileLength (
  IN  UINTN  FileHandle,
  OUT UINTN  *Length
  );

/**
  Get a temporary name for a file from the host running the debug agent.

  @param[out]  Buffer      Pointer to the buffer where the temporary name has to
                           be stored
  @param[in]   Identifier  File name identifier (integer in the range 0 to 255)
  @param[in]   Length      Length of the buffer to store the temporary name

  @retval  RETURN_SUCCESS            Temporary name returned
  @retval  RETURN_INVALID_PARAMETER  Invalid buffer address
  @retval  RETURN_ABORTED            Temporary name not returned

**/
RETURN_STATUS
SemihostFileTmpName(
  OUT  VOID   *Buffer,
  IN   UINT8  Identifier,
  IN   UINTN  Length
  );

RETURN_STATUS
SemihostFileRemove (
  IN CHAR8 *FileName
  );

/**
  Rename a specified file.

  @param[in]  FileName     Name of the file to rename.
  @param[in]  NewFileName  The new name of the file.

  @retval  RETURN_SUCCESS            File Renamed
  @retval  RETURN_INVALID_PARAMETER  Either the current or the new name is not specified
  @retval  RETURN_ABORTED            Rename failed

**/
RETURN_STATUS
SemihostFileRename(
  IN  CHAR8  *FileName,
  IN  CHAR8  *NewFileName
  );

CHAR8
SemihostReadCharacter (
  VOID
  );

VOID
SemihostWriteCharacter (
  IN CHAR8 Character
  );

VOID
SemihostWriteString (
  IN CHAR8 *String
  );

UINT32
SemihostSystem (
  IN CHAR8 *CommandLine
  );

#endif // __SEMIHOSTING_H__
