/** @file

Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2016 - 2018, ARM Limited. All rights reserved.<BR>

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Library/FvLib.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  (ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1))

/**
  Returns the highest bit set of the State field

  @param ErasePolarity   Erase Polarity  as defined by EFI_FVB_ERASE_POLARITY
                         in the Attributes field.
  @param FfsHeader       Pointer to FFS File Header.

  @return the highest bit in the State field
**/
EFI_FFS_FILE_STATE
GetFileState (
  IN UINT8                ErasePolarity,
  IN EFI_FFS_FILE_HEADER  *FfsHeader
  )
{
  EFI_FFS_FILE_STATE  FileState;
  EFI_FFS_FILE_STATE  HighestBit;

  FileState = FfsHeader->State;

  if (ErasePolarity != 0) {
    FileState = (EFI_FFS_FILE_STATE)~FileState;
  }

  HighestBit = 0x80;
  while (HighestBit != 0 && (HighestBit & FileState) == 0) {
    HighestBit >>= 1;
  }

  return HighestBit;
}

/**
  Calculates the checksum of the header of a file.

  @param FileHeader       Pointer to FFS File Header.

  @return Checksum of the header.
**/
UINT8
CalculateHeaderChecksum (
  IN EFI_FFS_FILE_HEADER  *FileHeader
  )
{
  UINT8 *ptr;
  UINTN Index;
  UINT8 Sum;

  Sum = 0;
  ptr = (UINT8 *) FileHeader;

  for (Index = 0; Index < sizeof (EFI_FFS_FILE_HEADER) - 3; Index += 4) {
    Sum = (UINT8) (Sum + ptr[Index]);
    Sum = (UINT8) (Sum + ptr[Index + 1]);
    Sum = (UINT8) (Sum + ptr[Index + 2]);
    Sum = (UINT8) (Sum + ptr[Index + 3]);
  }

  for (; Index < sizeof (EFI_FFS_FILE_HEADER); Index++) {
    Sum = (UINT8) (Sum + ptr[Index]);
  }
  //
  // State field (since this indicates the different state of file).
  //
  Sum = (UINT8) (Sum - FileHeader->State);
  //
  // Checksum field of the file is not part of the header checksum.
  //
  Sum = (UINT8) (Sum - FileHeader->IntegrityCheck.Checksum.File);

  return Sum;
}

/**
  Given the input file pointer, search for the next matching file in the
  FFS volume as defined by SearchType. The search starts from FileHeader inside
  the Firmware Volume defined by FwVolHeader.

  @param  SearchType  Filter to find only files of this type.
                      Type EFI_FV_FILETYPE_ALL causes no filtering to be done.
  @param  FwVolHeader Pointer to the FV header of the volume to search.
                      This parameter must point to a valid FFS volume.
  @param  FileHeader  Pointer to the current file from which to begin searching.
                      This pointer will be updated upon return to reflect the file found.

  @retval EFI_NOT_FOUND  No files matching the search criteria were found
  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
FfsFindNextFile (
  IN EFI_FV_FILETYPE             SearchType,
  IN EFI_FIRMWARE_VOLUME_HEADER  *FwVolHeader,
  IN OUT EFI_FFS_FILE_HEADER     **FileHeader
  )
{
  EFI_FIRMWARE_VOLUME_EXT_HEADER *FvExtHeader;

  EFI_FFS_FILE_HEADER *FfsFileHeader;
  UINT32              FileLength;
  UINT32              FileOccupiedSize;
  UINT32              FileOffset;
  UINT64              FvLength;
  UINT8               ErasePolarity;
  UINT8               FileState;

  FvLength = FwVolHeader->FvLength;
  if (FwVolHeader->Attributes & EFI_FVB2_ERASE_POLARITY) {
    ErasePolarity = 1;
  } else {
    ErasePolarity = 0;
  }
  //
  // If FileHeader is not specified (NULL) start with the first file in the
  // firmware volume.  Otherwise, start from the FileHeader.
  //
  if (*FileHeader == NULL) {

    if (FwVolHeader->ExtHeaderOffset != 0) {

      FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)((UINT8 *)FwVolHeader +
                                                       FwVolHeader->ExtHeaderOffset);

      FfsFileHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FvExtHeader +
                                              FvExtHeader->ExtHeaderSize);

    } else {

      FfsFileHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FwVolHeader +
                                              FwVolHeader->HeaderLength);

    }

    FfsFileHeader = (EFI_FFS_FILE_HEADER *)((UINTN)FwVolHeader +
                                            ALIGN_VALUE((UINTN)FfsFileHeader -
                                                        (UINTN)FwVolHeader, 8));
  } else {
    //
    // Length is 24 bits wide so mask upper 8 bits
    // FileLength is adjusted to FileOccupiedSize as it is 8 byte aligned.
    //
    FileLength        = FFS_FILE_SIZE(*FileHeader);
    FileOccupiedSize  = GET_OCCUPIED_SIZE (FileLength, 8);
    FfsFileHeader     = (EFI_FFS_FILE_HEADER *) ((UINT8 *) *FileHeader + FileOccupiedSize);
  }

  FileOffset = (UINT32) ((UINT8 *) FfsFileHeader - (UINT8 *) FwVolHeader);

  while (FileOffset < (FvLength - sizeof (EFI_FFS_FILE_HEADER))) {
    //
    // Get FileState which is the highest bit of the State
    //
    FileState = GetFileState (ErasePolarity, FfsFileHeader);

    switch (FileState) {

    case EFI_FILE_HEADER_INVALID:
      FileOffset += sizeof (EFI_FFS_FILE_HEADER);
      FfsFileHeader = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FfsFileHeader + sizeof (EFI_FFS_FILE_HEADER));
      break;

    case EFI_FILE_DATA_VALID:
    case EFI_FILE_MARKED_FOR_UPDATE:
      if (CalculateHeaderChecksum (FfsFileHeader) == 0) {
        FileLength        = FFS_FILE_SIZE(FfsFileHeader);
        FileOccupiedSize  = GET_OCCUPIED_SIZE (FileLength, 8);

        if ((SearchType == FfsFileHeader->Type) || (SearchType == EFI_FV_FILETYPE_ALL)) {

          *FileHeader = FfsFileHeader;

          return EFI_SUCCESS;
        }

        FileOffset += FileOccupiedSize;
        FfsFileHeader = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FfsFileHeader + FileOccupiedSize);
      } else {
        return EFI_NOT_FOUND;
      }
      break;

    case EFI_FILE_DELETED:
      FileLength        = FFS_FILE_SIZE(FfsFileHeader);
      FileOccupiedSize  = GET_OCCUPIED_SIZE (FileLength, 8);
      FileOffset += FileOccupiedSize;
      FfsFileHeader = (EFI_FFS_FILE_HEADER *) ((UINT8 *) FfsFileHeader + FileOccupiedSize);
      break;

    default:
      return EFI_NOT_FOUND;

    }
  }

  return EFI_NOT_FOUND;
}

/**
  Locates a section within a series of sections
  with the specified section type.

  @param[in]   Sections        The sections to search
  @param[in]   SizeOfSections  Total size of all sections
  @param[in]   SectionType     The section type to locate
  @param[out]  FoundSection    The FFS section if found

  @retval EFI_SUCCESS           The file and section was found
  @retval EFI_NOT_FOUND         The file and section was not found
  @retval EFI_VOLUME_CORRUPTED  The firmware volume was corrupted
**/
EFI_STATUS
EFIAPI
FindFfsSectionInSections (
  IN  VOID                             *Sections,
  IN  UINTN                            SizeOfSections,
  IN  EFI_SECTION_TYPE                 SectionType,
  OUT EFI_COMMON_SECTION_HEADER        **FoundSection
  )
{
  EFI_PHYSICAL_ADDRESS        CurrentAddress;
  UINT32                      Size;
  EFI_PHYSICAL_ADDRESS        EndOfSections;
  EFI_COMMON_SECTION_HEADER   *Section;
  EFI_PHYSICAL_ADDRESS        EndOfSection;

  //
  // Loop through the FFS file sections
  //
  EndOfSection = (EFI_PHYSICAL_ADDRESS)(UINTN) Sections;
  EndOfSections = EndOfSection + SizeOfSections;
  for (;;) {
    if (EndOfSection == EndOfSections) {
      break;
    }
    CurrentAddress = EndOfSection;

    Section = (EFI_COMMON_SECTION_HEADER*)(UINTN) CurrentAddress;

    Size = SECTION_SIZE (Section);
    if (Size < sizeof (*Section)) {
      return EFI_VOLUME_CORRUPTED;
    }

    EndOfSection = CurrentAddress + Size;
    if (EndOfSection > EndOfSections) {
      return EFI_VOLUME_CORRUPTED;
    }
    Size = GET_OCCUPIED_SIZE (Size, 4);

    //
    // Look for the requested section type
    //
    if (Section->Type == SectionType) {
      *FoundSection = Section;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Given the input file pointer, search for the next matching section in the
  FFS volume.

  @param  SearchType    Filter to find only sections of this type.
  @param  FfsFileHeader Pointer to the current file to search.
  @param  SectionHeader Pointer to the Section matching SectionType in FfsFileHeader.
                        NULL if section not found

  @retval  EFI_NOT_FOUND  No files matching the search criteria were found
  @retval  EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
FfsFindSection (
  IN EFI_SECTION_TYPE              SectionType,
  IN EFI_FFS_FILE_HEADER           *FfsFileHeader,
  IN OUT EFI_COMMON_SECTION_HEADER **SectionHeader
  )
{
  UINT32                    FileSize;
  EFI_COMMON_SECTION_HEADER *Section;
  EFI_STATUS                Status;

  //
  // Size is 24 bits wide so mask upper 8 bits.
  //    Does not include FfsFileHeader header size
  // FileSize is adjusted to FileOccupiedSize as it is 8 byte aligned.
  //
  Section   = (EFI_COMMON_SECTION_HEADER *) (FfsFileHeader + 1);
  FileSize  = FFS_FILE_SIZE(FfsFileHeader);
  FileSize -= sizeof (EFI_FFS_FILE_HEADER);

  Status = FindFfsSectionInSections (
             Section,
             FileSize,
             SectionType,
             SectionHeader
             );
  return Status;
}

/**
  Given the input file pointer, search for the next matching section in the
  FFS volume.

  @param  SearchType      Filter to find only sections of this type.
  @param  FfsFileHeader   Pointer to the current file to search.
  @param  SectionData     Pointer to the Section matching SectionType in FfsFileHeader.
                          NULL if section not found
  @param  SectionDataSize The size of SectionData

  @retval  EFI_NOT_FOUND  No files matching the search criteria were found
  @retval  EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
FfsFindSectionData (
  IN EFI_SECTION_TYPE      SectionType,
  IN EFI_FFS_FILE_HEADER   *FfsFileHeader,
  IN OUT VOID              **SectionData,
  IN OUT UINTN             *SectionDataSize
  )
{
  UINT32                    FileSize;
  EFI_COMMON_SECTION_HEADER *Section;
  UINT32                    SectionLength;
  UINT32                    ParsedLength;

  //
  // Size is 24 bits wide so mask upper 8 bits.
  // Does not include FfsFileHeader header size
  // FileSize is adjusted to FileOccupiedSize as it is 8 byte aligned.
  //
  Section   = (EFI_COMMON_SECTION_HEADER *) (FfsFileHeader + 1);
  FileSize  = FFS_FILE_SIZE(FfsFileHeader);
  FileSize -= sizeof (EFI_FFS_FILE_HEADER);

  *SectionData  = NULL;
  ParsedLength  = 0;
  while (ParsedLength < FileSize) {
    if (Section->Type == SectionType) {
      *SectionData = (VOID *) (Section + 1);
      *SectionDataSize = SECTION_SIZE(Section);
      return EFI_SUCCESS;
    }
    //
    // Size is 24 bits wide so mask upper 8 bits.
    // SectionLength is adjusted it is 4 byte aligned.
    // Go to the next section
    //
    SectionLength = SECTION_SIZE(Section);
    SectionLength = GET_OCCUPIED_SIZE (SectionLength, 4);

    ParsedLength += SectionLength;
    Section = (EFI_COMMON_SECTION_HEADER *) ((UINT8 *) Section + SectionLength);
  }

  return EFI_NOT_FOUND;
}
