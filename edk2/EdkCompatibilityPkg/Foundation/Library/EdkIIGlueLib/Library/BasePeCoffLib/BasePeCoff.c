/*++

Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  EdkIIGlueDxeDriverEntryPoint.c
  
Abstract: 

  Pe/Coff loader

--*/

#include "BasePeCoffLibInternals.h"

/**
  Retrieves the magic value from the PE/COFF header.

  @param  Hdr             The buffer in which to return the PE32, PE32+, or TE header.

  @return EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC - Image is PE32
  @return EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC - Image is PE32+

**/
UINT16
PeCoffLoaderGetPeHeaderMagicValue (
  IN  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr
  )
{
  //
  // NOTE: Some versions of Linux ELILO for Itanium have an incorrect magic value 
  //       in the PE/COFF Header.  If the MachineType is Itanium(IA64) and the 
  //       Magic value in the OptionalHeader is  EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC
  //       then override the returned value to EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC
  //
  if (Hdr.Pe32->FileHeader.Machine == EFI_IMAGE_MACHINE_IA64 && Hdr.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    return EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
  }
  //
  // Return the magic value from the PC/COFF Optional Header
  //
  return Hdr.Pe32->OptionalHeader.Magic;
}


/**
  Retrieves the PE or TE Header from a PE/COFF or TE image.

  @param  ImageContext    The context of the image being loaded.
  @param  Hdr             The buffer in which to return the PE32, PE32+, or TE header.

  @retval RETURN_SUCCESS  The PE or TE Header is read.
  @retval Other           The error status from reading the PE/COFF or TE image using the ImageRead function.

**/
RETURN_STATUS
GluePeCoffLoaderGetPeHeader (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT         *ImageContext,
  OUT    EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr
  )
{
  RETURN_STATUS         Status;
  EFI_IMAGE_DOS_HEADER  DosHdr;
  UINTN                 Size;
  UINT16                Magic;

  //
  // Read the DOS image header to check for it's existance
  //
  Size = sizeof (EFI_IMAGE_DOS_HEADER);
  Status = ImageContext->ImageRead (
                           ImageContext->Handle,
                           0,
                           &Size,
                           &DosHdr
                           );
  if (RETURN_ERROR (Status)) {
    ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
    return Status;
  }

  ImageContext->PeCoffHeaderOffset = 0;
  if (DosHdr.e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // DOS image header is present, so read the PE header after the DOS image
    // header
    //
    ImageContext->PeCoffHeaderOffset = DosHdr.e_lfanew;
  }

  //
  // Read the PE/COFF Header. For PE32 (32-bit) this will read in too much
  // data, but that should not hurt anythine. Hdr.Pe32->OptionalHeader.Magic
  // determins if this is a PE32 or PE32+ image. The magic is in the same
  // location in both images.
  //
  Size = sizeof (EFI_IMAGE_OPTIONAL_HEADER_UNION);
  Status = ImageContext->ImageRead (
                           ImageContext->Handle,
                           ImageContext->PeCoffHeaderOffset,
                           &Size,
                           Hdr.Pe32
                           );
  if (RETURN_ERROR (Status)) {
    ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
    return Status;
  }

  //
  // Use Signature to figure out if we understand the image format
  //
  if (Hdr.Te->Signature == EFI_TE_IMAGE_HEADER_SIGNATURE) {
    ImageContext->IsTeImage         = TRUE;
    ImageContext->Machine           = Hdr.Te->Machine;
    ImageContext->ImageType         = (UINT16)(Hdr.Te->Subsystem);
    ImageContext->ImageSize         = 0;
    ImageContext->SectionAlignment  = 4096;
    ImageContext->SizeOfHeaders     = sizeof (EFI_TE_IMAGE_HEADER) + (UINTN)Hdr.Te->BaseOfCode - (UINTN)Hdr.Te->StrippedSize;

  } else if (Hdr.Pe32->Signature == EFI_IMAGE_NT_SIGNATURE)  {
    ImageContext->IsTeImage = FALSE;
    ImageContext->Machine = Hdr.Pe32->FileHeader.Machine;

    Magic = PeCoffLoaderGetPeHeaderMagicValue (Hdr);

    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      ImageContext->ImageType         = Hdr.Pe32->OptionalHeader.Subsystem;
      ImageContext->ImageSize         = (UINT64)Hdr.Pe32->OptionalHeader.SizeOfImage;
      ImageContext->SectionAlignment  = Hdr.Pe32->OptionalHeader.SectionAlignment;
      ImageContext->SizeOfHeaders     = Hdr.Pe32->OptionalHeader.SizeOfHeaders;

    } else if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
      //
      // Use PE32+ offset
      //
      ImageContext->ImageType         = Hdr.Pe32Plus->OptionalHeader.Subsystem;
      ImageContext->ImageSize         = (UINT64) Hdr.Pe32Plus->OptionalHeader.SizeOfImage;
      ImageContext->SectionAlignment  = Hdr.Pe32Plus->OptionalHeader.SectionAlignment;
      ImageContext->SizeOfHeaders     = Hdr.Pe32Plus->OptionalHeader.SizeOfHeaders;
    } else {
      ImageContext->ImageError = IMAGE_ERROR_INVALID_MACHINE_TYPE;
      return RETURN_UNSUPPORTED;
    }
  } else {
    ImageContext->ImageError = IMAGE_ERROR_INVALID_MACHINE_TYPE;
    return RETURN_UNSUPPORTED;
  }

  if (!PeCoffLoaderImageFormatSupported (ImageContext->Machine)) {
    //
    // If the PE/COFF loader does not support the image type return
    // unsupported. This library can suport lots of types of images
    // this does not mean the user of this library can call the entry
    // point of the image.
    //
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}


/**
  Retrieves information about a PE/COFF image.

  Computes the PeCoffHeaderOffset, ImageAddress, ImageSize, DestinationAddress, CodeView,
  PdbPointer, RelocationsStripped, SectionAlignment, SizeOfHeaders, and DebugDirectoryEntryRva
  fields of the ImageContext structure.  If ImageContext is NULL, then return RETURN_INVALID_PARAMETER.
  If the PE/COFF image accessed through the ImageRead service in the ImageContext structure is not
  a supported PE/COFF image type, then return RETURN_UNSUPPORTED.  If any errors occur while
  computing the fields of ImageContext, then the error status is returned in the ImageError field of
  ImageContext.

  @param  ImageContext              Pointer to the image context structure that describes the PE/COFF
                                    image that needs to be examined by this function.

  @retval RETURN_SUCCESS            The information on the PE/COFF image was collected.
  @retval RETURN_INVALID_PARAMETER  ImageContext is NULL.
  @retval RETURN_UNSUPPORTED        The PE/COFF image is not supported.

**/
RETURN_STATUS
EFIAPI
GluePeCoffLoaderGetImageInfo (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT           *ImageContext
  )
{
  RETURN_STATUS                         Status;
  EFI_IMAGE_OPTIONAL_HEADER_UNION       HdrData;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr;
  EFI_IMAGE_DATA_DIRECTORY              *DebugDirectoryEntry;
  UINTN                                 Size;
  UINTN                                 Index;
  UINTN                                 DebugDirectoryEntryRva;
  UINTN                                 DebugDirectoryEntryFileOffset;
  UINTN                                 SectionHeaderOffset;
  EFI_IMAGE_SECTION_HEADER              SectionHeader;
  EFI_IMAGE_DEBUG_DIRECTORY_ENTRY       DebugEntry;
  UINT32                                NumberOfRvaAndSizes;
  UINT16                                Magic;

  if (NULL == ImageContext) {
    return RETURN_INVALID_PARAMETER;
  }
  //
  // Assume success
  //
  ImageContext->ImageError  = IMAGE_ERROR_SUCCESS;

  Hdr.Union = &HdrData;
  Status = PeCoffLoaderGetPeHeader (ImageContext, Hdr);
  if (RETURN_ERROR (Status)) {
    return Status;
  }

  Magic = PeCoffLoaderGetPeHeaderMagicValue (Hdr);

  //
  // Retrieve the base address of the image
  //
  if (!(ImageContext->IsTeImage)) {
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      ImageContext->ImageAddress = Hdr.Pe32->OptionalHeader.ImageBase;
    } else {
      //
      // Use PE32+ offset
      //
      ImageContext->ImageAddress = Hdr.Pe32Plus->OptionalHeader.ImageBase;
    }
  } else {
    ImageContext->ImageAddress = (PHYSICAL_ADDRESS)(Hdr.Te->ImageBase + Hdr.Te->StrippedSize - sizeof (EFI_TE_IMAGE_HEADER));
  }

  //
  // Initialize the alternate destination address to 0 indicating that it
  // should not be used.
  //
  ImageContext->DestinationAddress = 0;

  //
  // Initialize the codeview pointer.
  //
  ImageContext->CodeView    = NULL;
  ImageContext->PdbPointer  = NULL;

  //
  // Three cases with regards to relocations:
  // - Image has base relocs, RELOCS_STRIPPED==0    => image is relocatable
  // - Image has no base relocs, RELOCS_STRIPPED==1 => Image is not relocatable
  // - Image has no base relocs, RELOCS_STRIPPED==0 => Image is relocatable but
  //   has no base relocs to apply
  // Obviously having base relocations with RELOCS_STRIPPED==1 is invalid.
  //
  // Look at the file header to determine if relocations have been stripped, and
  // save this info in the image context for later use.
  //
  if ((!(ImageContext->IsTeImage)) && ((Hdr.Pe32->FileHeader.Characteristics & EFI_IMAGE_FILE_RELOCS_STRIPPED) != 0)) {
    ImageContext->RelocationsStripped = TRUE;
  } else {
    ImageContext->RelocationsStripped = FALSE;
  }

  if (!(ImageContext->IsTeImage)) {
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      NumberOfRvaAndSizes = Hdr.Pe32->OptionalHeader.NumberOfRvaAndSizes;
      DebugDirectoryEntry = (EFI_IMAGE_DATA_DIRECTORY *)&(Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG]);
    } else {
      //
      // Use PE32+ offset
      //
      NumberOfRvaAndSizes = Hdr.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
      DebugDirectoryEntry = (EFI_IMAGE_DATA_DIRECTORY *)&(Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_DEBUG]);
    }

    if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_DEBUG) {

      DebugDirectoryEntryRva = DebugDirectoryEntry->VirtualAddress;

      //
      // Determine the file offset of the debug directory...  This means we walk
      // the sections to find which section contains the RVA of the debug
      // directory
      //
      DebugDirectoryEntryFileOffset = 0;

      SectionHeaderOffset = (UINTN)(
                               ImageContext->PeCoffHeaderOffset +
                               sizeof (UINT32) +
                               sizeof (EFI_IMAGE_FILE_HEADER) +
                               Hdr.Pe32->FileHeader.SizeOfOptionalHeader
                               );

      for (Index = 0; Index < Hdr.Pe32->FileHeader.NumberOfSections; Index++) {
        //
        // Read section header from file
        //
        Size = sizeof (EFI_IMAGE_SECTION_HEADER);
        Status = ImageContext->ImageRead (
                                 ImageContext->Handle,
                                 SectionHeaderOffset,
                                 &Size,
                                 &SectionHeader
                                 );
        if (RETURN_ERROR (Status)) {
          ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
          return Status;
        }

        if (DebugDirectoryEntryRva >= SectionHeader.VirtualAddress &&
            DebugDirectoryEntryRva < SectionHeader.VirtualAddress + SectionHeader.Misc.VirtualSize) {

          DebugDirectoryEntryFileOffset = DebugDirectoryEntryRva - SectionHeader.VirtualAddress + SectionHeader.PointerToRawData;
          break;
        }

        SectionHeaderOffset += sizeof (EFI_IMAGE_SECTION_HEADER);
      }

      if (DebugDirectoryEntryFileOffset != 0) {
        for (Index = 0; Index < DebugDirectoryEntry->Size; Index += sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY)) {
          //
          // Read next debug directory entry
          //
          Size = sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY);
          Status = ImageContext->ImageRead (
                                   ImageContext->Handle,
                                   DebugDirectoryEntryFileOffset,
                                   &Size,
                                   &DebugEntry
                                   );
          if (RETURN_ERROR (Status)) {
            ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
            return Status;
          }
          if (DebugEntry.Type == EFI_IMAGE_DEBUG_TYPE_CODEVIEW) {
            ImageContext->DebugDirectoryEntryRva = (UINT32) (DebugDirectoryEntryRva + Index);
            if (DebugEntry.RVA == 0 && DebugEntry.FileOffset != 0) {
              ImageContext->ImageSize += DebugEntry.SizeOfData;
            }

            return RETURN_SUCCESS;
          }
        }
      }
    }
  } else {

    DebugDirectoryEntry             = &Hdr.Te->DataDirectory[1];
    DebugDirectoryEntryRva          = DebugDirectoryEntry->VirtualAddress;
    SectionHeaderOffset             = (UINTN)(sizeof (EFI_TE_IMAGE_HEADER));

    DebugDirectoryEntryFileOffset   = 0;

    for (Index = 0; Index < Hdr.Te->NumberOfSections;) {
      //
      // Read section header from file
      //
      Size   = sizeof (EFI_IMAGE_SECTION_HEADER);
      Status = ImageContext->ImageRead (
                               ImageContext->Handle,
                               SectionHeaderOffset,
                               &Size,
                               &SectionHeader
                               );
      if (RETURN_ERROR (Status)) {
        ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
        return Status;
      }

      if (DebugDirectoryEntryRva >= SectionHeader.VirtualAddress &&
          DebugDirectoryEntryRva < SectionHeader.VirtualAddress + SectionHeader.Misc.VirtualSize) {
        DebugDirectoryEntryFileOffset = DebugDirectoryEntryRva -
                                        SectionHeader.VirtualAddress +
                                        SectionHeader.PointerToRawData +
                                        sizeof (EFI_TE_IMAGE_HEADER) -
                                        Hdr.Te->StrippedSize;

        //
        // File offset of the debug directory was found, if this is not the last
        // section, then skip to the last section for calculating the image size.
        //
        if (Index < (UINTN) Hdr.Te->NumberOfSections - 1) {
          SectionHeaderOffset += (Hdr.Te->NumberOfSections - 1 - Index) * sizeof (EFI_IMAGE_SECTION_HEADER);
          Index = Hdr.Te->NumberOfSections - 1;
          continue;
        }
      }

      //
      // In Te image header there is not a field to describe the ImageSize.
      // Actually, the ImageSize equals the RVA plus the VirtualSize of
      // the last section mapped into memory (Must be rounded up to
      // a mulitple of Section Alignment). Per the PE/COFF specification, the
      // section headers in the Section Table must appear in order of the RVA
      // values for the corresponding sections. So the ImageSize can be determined
      // by the RVA and the VirtualSize of the last section header in the
      // Section Table.
      //
      if ((++Index) == (UINTN)Hdr.Te->NumberOfSections) {
        ImageContext->ImageSize = (SectionHeader.VirtualAddress + SectionHeader.Misc.VirtualSize +
                                   ImageContext->SectionAlignment - 1) & ~(ImageContext->SectionAlignment - 1);
      }

      SectionHeaderOffset += sizeof (EFI_IMAGE_SECTION_HEADER);
    }

    if (DebugDirectoryEntryFileOffset != 0) {
      for (Index = 0; Index < DebugDirectoryEntry->Size; Index += sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY)) {
        //
        // Read next debug directory entry
        //
        Size = sizeof (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY);
        Status = ImageContext->ImageRead (
                                 ImageContext->Handle,
                                 DebugDirectoryEntryFileOffset,
                                 &Size,
                                 &DebugEntry
                                 );
        if (RETURN_ERROR (Status)) {
          ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
          return Status;
        }

        if (DebugEntry.Type == EFI_IMAGE_DEBUG_TYPE_CODEVIEW) {
          ImageContext->DebugDirectoryEntryRva = (UINT32) (DebugDirectoryEntryRva + Index);
          return RETURN_SUCCESS;
        }
      }
    }
  }

  return RETURN_SUCCESS;
}


/**
  Converts an image address to the loaded address.

  @param  ImageContext  The context of the image being loaded.
  @param  Address       The address to be converted to the loaded address.

  @return The converted address or NULL if the address can not be converted.

**/
VOID *
GluePeCoffLoaderImageAddress (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT          *ImageContext,
  IN     UINTN                                 Address
  )
{
  //
  // @bug Check to make sure ImageSize is correct for the relocated image.
  //      it may only work for the file we start with and not the relocated image
  //
  if (Address >= ImageContext->ImageSize) {
    ImageContext->ImageError = IMAGE_ERROR_INVALID_IMAGE_ADDRESS;
    return NULL;
  }

  return (CHAR8 *)((UINTN) ImageContext->ImageAddress + Address);
}

/**
  Applies relocation fixups to a PE/COFF image that was loaded with PeCoffLoaderLoadImage().

  If the DestinationAddress field of ImageContext is 0, then use the ImageAddress field of
  ImageContext as the relocation base address.  Otherwise, use the DestinationAddress field
  of ImageContext as the relocation base address.  The caller must allocate the relocation
  fixup log buffer and fill in the FixupData field of ImageContext prior to calling this function.
  If ImageContext is NULL, then ASSERT().

  @param  ImageContext        Pointer to the image context structure that describes the PE/COFF
                              image that is being relocated.

  @retval RETURN_SUCCESS      The PE/COFF image was relocated.
                              Extended status information is in the ImageError field of ImageContext.
  @retval RETURN_LOAD_ERROR   The image in not a valid PE/COFF image.
                              Extended status information is in the ImageError field of ImageContext.
  @retval RETURN_UNSUPPORTED  A relocation record type is not supported.
                              Extended status information is in the ImageError field of ImageContext.

**/
RETURN_STATUS
EFIAPI
GluePeCoffLoaderRelocateImage (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext
  )
{
  RETURN_STATUS                         Status;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr;
  EFI_IMAGE_DATA_DIRECTORY              *RelocDir;
  UINT64                                Adjust;
  EFI_IMAGE_BASE_RELOCATION             *RelocBase;
  EFI_IMAGE_BASE_RELOCATION             *RelocBaseEnd;
  UINT16                                *Reloc;
  UINT16                                *RelocEnd;
  CHAR8                                 *Fixup;
  CHAR8                                 *FixupBase;
  UINT16                                *F16;
  UINT32                                *F32;
  UINT64                                *F64;
  CHAR8                                 *FixupData;
  PHYSICAL_ADDRESS                      BaseAddress;
  UINT32                                NumberOfRvaAndSizes;
  UINT16                                Magic;

  ASSERT (ImageContext != NULL);

  //
  // Assume success
  //
  ImageContext->ImageError = IMAGE_ERROR_SUCCESS;

  //
  // If there are no relocation entries, then we are done
  //
  if (ImageContext->RelocationsStripped) {
    return RETURN_SUCCESS;
  }

  //
  // If the destination address is not 0, use that rather than the
  // image address as the relocation target.
  //
  if (ImageContext->DestinationAddress != 0) {
    BaseAddress = ImageContext->DestinationAddress;
  } else if (!(ImageContext->IsTeImage)) {
    BaseAddress = ImageContext->ImageAddress;
  } else {
    Hdr.Te      = (EFI_TE_IMAGE_HEADER *)(UINTN)(ImageContext->ImageAddress);
    BaseAddress = ImageContext->ImageAddress + sizeof (EFI_TE_IMAGE_HEADER) - Hdr.Te->StrippedSize; 
  }

  if (!(ImageContext->IsTeImage)) {
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINTN)ImageContext->ImageAddress + ImageContext->PeCoffHeaderOffset);

    Magic = PeCoffLoaderGetPeHeaderMagicValue (Hdr);

    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      Adjust = (UINT64)BaseAddress - Hdr.Pe32->OptionalHeader.ImageBase;
      Hdr.Pe32->OptionalHeader.ImageBase = (UINT32)BaseAddress;

      NumberOfRvaAndSizes = Hdr.Pe32->OptionalHeader.NumberOfRvaAndSizes;
      RelocDir  = &Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC];
    } else {
      //
      // Use PE32+ offset
      //
      Adjust = (UINT64) BaseAddress - Hdr.Pe32Plus->OptionalHeader.ImageBase;
      Hdr.Pe32Plus->OptionalHeader.ImageBase = (UINT64)BaseAddress;

      NumberOfRvaAndSizes = Hdr.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
      RelocDir  = &Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC];
    }

    //
    // Find the relocation block
    // Per the PE/COFF spec, you can't assume that a given data directory
    // is present in the image. You have to check the NumberOfRvaAndSizes in
    // the optional header to verify a desired directory entry is there.
    //

    if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC && RelocDir->Size > 0) {
      RelocBase = PeCoffLoaderImageAddress (ImageContext, RelocDir->VirtualAddress);
      RelocBaseEnd = PeCoffLoaderImageAddress (
                      ImageContext,
                      RelocDir->VirtualAddress + RelocDir->Size - 1
                      );
      if ((RelocBase == NULL) || (RelocBaseEnd == NULL)) {
        //
        // If the base start or end address resolved to 0, then fail.
        //
        return RETURN_LOAD_ERROR;
      }
    } else {
      //
      // Set base and end to bypass processing below.
      //
      RelocBase = RelocBaseEnd = 0;
    }
  } else {
    Hdr.Te             = (EFI_TE_IMAGE_HEADER *)(UINTN)(ImageContext->ImageAddress);
    Adjust             = (UINT64) (BaseAddress - Hdr.Te->ImageBase);
    Hdr.Te->ImageBase  = (UINT64) (BaseAddress);

    //
    // Find the relocation block
    //
    RelocDir = &Hdr.Te->DataDirectory[0];
    if (RelocDir->Size > 0) {
      RelocBase = (EFI_IMAGE_BASE_RELOCATION *)(UINTN)(
                                      ImageContext->ImageAddress +
                                      RelocDir->VirtualAddress +
                                      sizeof(EFI_TE_IMAGE_HEADER) -
                                      Hdr.Te->StrippedSize
                                      );
      RelocBaseEnd = (EFI_IMAGE_BASE_RELOCATION *) ((UINTN) RelocBase + (UINTN) RelocDir->Size - 1);
    } else {
      //
      // Set base and end to bypass processing below.
      //
      RelocBase = NULL;
      RelocBaseEnd = NULL;
    }
  }

  //
  // Run the relocation information and apply the fixups
  //
  FixupData = ImageContext->FixupData;
  while (RelocBase < RelocBaseEnd) {

    Reloc     = (UINT16 *) ((CHAR8 *) RelocBase + sizeof (EFI_IMAGE_BASE_RELOCATION));
    RelocEnd  = (UINT16 *) ((CHAR8 *) RelocBase + RelocBase->SizeOfBlock);
    if (!(ImageContext->IsTeImage)) {
      FixupBase = PeCoffLoaderImageAddress (ImageContext, RelocBase->VirtualAddress);

      if (FixupBase == NULL) {
        //
        // If the FixupBase address resolved to 0, then fail.
        //
        return RETURN_LOAD_ERROR;
      }
    } else {
      FixupBase = (CHAR8 *)(UINTN)(ImageContext->ImageAddress +
                    RelocBase->VirtualAddress +
                    sizeof(EFI_TE_IMAGE_HEADER) -
                    Hdr.Te->StrippedSize
                    );
    }

    if ((CHAR8 *) RelocEnd < (CHAR8 *) ((UINTN) ImageContext->ImageAddress) ||
        (CHAR8 *) RelocEnd > (CHAR8 *)((UINTN)ImageContext->ImageAddress +
          (UINTN)ImageContext->ImageSize)) {
      ImageContext->ImageError = IMAGE_ERROR_FAILED_RELOCATION;
      return RETURN_LOAD_ERROR;
    }

    //
    // Run this relocation record
    //
    while (Reloc < RelocEnd) {

      Fixup = FixupBase + (*Reloc & 0xFFF);
      switch ((*Reloc) >> 12) {
      case EFI_IMAGE_REL_BASED_ABSOLUTE:
        break;

      case EFI_IMAGE_REL_BASED_HIGH:
        F16   = (UINT16 *) Fixup;
        *F16 = (UINT16) (*F16 + ((UINT16) ((UINT32) Adjust >> 16)));
        if (FixupData != NULL) {
          *(UINT16 *) FixupData = *F16;
          FixupData             = FixupData + sizeof (UINT16);
        }
        break;

      case EFI_IMAGE_REL_BASED_LOW:
        F16   = (UINT16 *) Fixup;
        *F16  = (UINT16) (*F16 + (UINT16) Adjust);
        if (FixupData != NULL) {
          *(UINT16 *) FixupData = *F16;
          FixupData             = FixupData + sizeof (UINT16);
        }
        break;

      case EFI_IMAGE_REL_BASED_HIGHLOW:
        F32   = (UINT32 *) Fixup;
        *F32  = *F32 + (UINT32) Adjust;
        if (FixupData != NULL) {
          FixupData             = ALIGN_POINTER (FixupData, sizeof (UINT32));
          *(UINT32 *)FixupData  = *F32;
          FixupData             = FixupData + sizeof (UINT32);
        }
        break;

      case EFI_IMAGE_REL_BASED_DIR64:
        F64 = (UINT64 *) Fixup;
        *F64 = *F64 + (UINT64) Adjust;
        if (FixupData != NULL) {
          FixupData = ALIGN_POINTER (FixupData, sizeof(UINT64));
          *(UINT64 *)(FixupData) = *F64;
          FixupData = FixupData + sizeof(UINT64);
        }
        break;

      default:
        //
        // The common code does not handle some of the stranger IPF relocations
        // PeCoffLoaderRelocateImageEx () addes support for these complex fixups
        // on IPF and is a No-Op on other archtiectures.
        //
        Status = PeCoffLoaderRelocateImageEx (Reloc, Fixup, &FixupData, Adjust);
        if (RETURN_ERROR (Status)) {
          ImageContext->ImageError = IMAGE_ERROR_FAILED_RELOCATION;
          return Status;
        }
      }

      //
      // Next relocation record
      //
      Reloc += 1;
    }

    //
    // Next reloc block
    //
    RelocBase = (EFI_IMAGE_BASE_RELOCATION *) RelocEnd;
  }

  return RETURN_SUCCESS;
}

/**
  Loads a PE/COFF image into memory.

  Loads the PE/COFF image accessed through the ImageRead service of ImageContext into the buffer
  specified by the ImageAddress and ImageSize fields of ImageContext.  The caller must allocate
  the load buffer and fill in the ImageAddress and ImageSize fields prior to calling this function.
  The EntryPoint, FixupDataSize, CodeView, and PdbPointer fields of ImageContext are computed.
  If ImageContext is NULL, then ASSERT().

  @param  ImageContext              Pointer to the image context structure that describes the PE/COFF
                                    image that is being loaded.

  @retval RETURN_SUCCESS            The PE/COFF image was loaded into the buffer specified by
                                    the ImageAddress and ImageSize fields of ImageContext.
                                    Extended status information is in the ImageError field of ImageContext.
  @retval RETURN_BUFFER_TOO_SMALL   The caller did not provide a large enough buffer.
                                    Extended status information is in the ImageError field of ImageContext.
  @retval RETURN_LOAD_ERROR         The PE/COFF image is an EFI Runtime image with no relocations.
                                    Extended status information is in the ImageError field of ImageContext.
  @retval RETURN_INVALID_PARAMETER  The image address is invalid.
                                    Extended status information is in the ImageError field of ImageContext.

**/
RETURN_STATUS
EFIAPI
GluePeCoffLoaderLoadImage (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT  *ImageContext
  )
{
  RETURN_STATUS                         Status;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION   Hdr;
  PE_COFF_LOADER_IMAGE_CONTEXT          CheckContext;
  EFI_IMAGE_SECTION_HEADER              *FirstSection;
  EFI_IMAGE_SECTION_HEADER              *Section;
  UINTN                                 NumberOfSections;
  UINTN                                 Index;
  CHAR8                                 *Base;
  CHAR8                                 *End;
  CHAR8                                 *MaxEnd;
  EFI_IMAGE_DATA_DIRECTORY              *DirectoryEntry;
  EFI_IMAGE_DEBUG_DIRECTORY_ENTRY       *DebugEntry;
  UINTN                                 Size;
  UINT32                                TempDebugEntryRva;
  UINT32                                NumberOfRvaAndSizes;
  UINT16                                Magic;

  ASSERT (ImageContext != NULL);

  //
  // Assume success
  //
  ImageContext->ImageError = IMAGE_ERROR_SUCCESS;

  //
  // Copy the provided context info into our local version, get what we
  // can from the original image, and then use that to make sure everything
  // is legit.
  //
  CopyMem (&CheckContext, ImageContext, sizeof (PE_COFF_LOADER_IMAGE_CONTEXT));

  Status = PeCoffLoaderGetImageInfo (&CheckContext);
  if (RETURN_ERROR (Status)) {
    return Status;
  }

  //
  // Make sure there is enough allocated space for the image being loaded
  //
  if (ImageContext->ImageSize < CheckContext.ImageSize) {
    ImageContext->ImageError = IMAGE_ERROR_INVALID_IMAGE_SIZE;
    return RETURN_BUFFER_TOO_SMALL;
  }
  if (ImageContext->ImageAddress == 0) {
    //
    // Image cannot be loaded into 0 address.
    //
    ImageContext->ImageError = IMAGE_ERROR_INVALID_IMAGE_ADDRESS;
    return RETURN_INVALID_PARAMETER;
  }
  //
  // If there's no relocations, then make sure it's not a runtime driver,
  // and that it's being loaded at the linked address.
  //
  if (CheckContext.RelocationsStripped) {
    //
    // If the image does not contain relocations and it is a runtime driver
    // then return an error.
    //
    if (CheckContext.ImageType == EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER) {
      ImageContext->ImageError = IMAGE_ERROR_INVALID_SUBSYSTEM;
      return RETURN_LOAD_ERROR;
    }
    //
    // If the image does not contain relocations, and the requested load address
    // is not the linked address, then return an error.
    //
    if (CheckContext.ImageAddress != ImageContext->ImageAddress) {
      ImageContext->ImageError = IMAGE_ERROR_INVALID_IMAGE_ADDRESS;
      return RETURN_INVALID_PARAMETER;
    }
  }
  //
  // Make sure the allocated space has the proper section alignment
  //
  if (!(ImageContext->IsTeImage)) {
    if ((ImageContext->ImageAddress & (CheckContext.SectionAlignment - 1)) != 0) {
      ImageContext->ImageError = IMAGE_ERROR_INVALID_SECTION_ALIGNMENT;
      return RETURN_INVALID_PARAMETER;
    }
  }
  //
  // Read the entire PE/COFF or TE header into memory
  //
  if (!(ImageContext->IsTeImage)) {
    Status = ImageContext->ImageRead (
                            ImageContext->Handle,
                            0,
                            &ImageContext->SizeOfHeaders,
                            (VOID *) (UINTN) ImageContext->ImageAddress
                            );

    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINTN)ImageContext->ImageAddress + ImageContext->PeCoffHeaderOffset);

    FirstSection = (EFI_IMAGE_SECTION_HEADER *) (
                      (UINTN)ImageContext->ImageAddress +
                      ImageContext->PeCoffHeaderOffset +
                      sizeof(UINT32) +
                      sizeof(EFI_IMAGE_FILE_HEADER) +
                      Hdr.Pe32->FileHeader.SizeOfOptionalHeader
      );
    NumberOfSections = (UINTN) (Hdr.Pe32->FileHeader.NumberOfSections);
  } else {
    Status = ImageContext->ImageRead (
                            ImageContext->Handle,
                            0,
                            &ImageContext->SizeOfHeaders,
                            (void *)(UINTN)ImageContext->ImageAddress
                            );

    Hdr.Te = (EFI_TE_IMAGE_HEADER *)(UINTN)(ImageContext->ImageAddress);

    FirstSection = (EFI_IMAGE_SECTION_HEADER *) (
                      (UINTN)ImageContext->ImageAddress +
                      sizeof(EFI_TE_IMAGE_HEADER)
                      );
    NumberOfSections  = (UINTN) (Hdr.Te->NumberOfSections);

  }

  if (RETURN_ERROR (Status)) {
    ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
    return RETURN_LOAD_ERROR;
  }

  //
  // Load each section of the image
  //
  Section = FirstSection;
  for (Index = 0, MaxEnd = NULL; Index < NumberOfSections; Index++) {

    //
    // Compute sections address
    //
    Base = PeCoffLoaderImageAddress (ImageContext, Section->VirtualAddress);
    End = PeCoffLoaderImageAddress (
            ImageContext,
            Section->VirtualAddress + Section->Misc.VirtualSize - 1
            );
    if (ImageContext->IsTeImage) {
      Base = (CHAR8 *)((UINTN) Base + sizeof (EFI_TE_IMAGE_HEADER) - (UINTN)Hdr.Te->StrippedSize);
      End  = (CHAR8 *)((UINTN) End +  sizeof (EFI_TE_IMAGE_HEADER) - (UINTN)Hdr.Te->StrippedSize);
    }

    if (End > MaxEnd) {
      MaxEnd = End;
    }
    //
    // If the base start or end address resolved to 0, then fail.
    //
    if ((Base == NULL) || (End == NULL)) {
      ImageContext->ImageError = IMAGE_ERROR_SECTION_NOT_LOADED;
      return RETURN_LOAD_ERROR;
    }

    //
    // Read the section
    //
    Size = (UINTN) Section->Misc.VirtualSize;
    if ((Size == 0) || (Size > Section->SizeOfRawData)) {
      Size = (UINTN) Section->SizeOfRawData;
    }

    if (Section->SizeOfRawData) {
      if (!(ImageContext->IsTeImage)) {
        Status = ImageContext->ImageRead (
                                ImageContext->Handle,
                                Section->PointerToRawData,
                                &Size,
                                Base
                                );
      } else {
        Status = ImageContext->ImageRead (
                                ImageContext->Handle,
                                Section->PointerToRawData + sizeof (EFI_TE_IMAGE_HEADER) - (UINTN)Hdr.Te->StrippedSize,
                                &Size,
                                Base
                                );
      }

      if (RETURN_ERROR (Status)) {
        ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
        return Status;
      }
    }

    //
    // If raw size is less then virt size, zero fill the remaining
    //

    if (Size < Section->Misc.VirtualSize) {
      ZeroMem (Base + Size, Section->Misc.VirtualSize - Size);
    }

    //
    // Next Section
    //
    Section += 1;
  }

  //
  // Get image's entry point
  //
  Magic = PeCoffLoaderGetPeHeaderMagicValue (Hdr);
  if (!(ImageContext->IsTeImage)) {
    //
    // Sizes of AddressOfEntryPoint are different so we need to do this safely
    //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      ImageContext->EntryPoint = (PHYSICAL_ADDRESS)(UINTN)PeCoffLoaderImageAddress (
                                                            ImageContext,
                                                            (UINTN)Hdr.Pe32->OptionalHeader.AddressOfEntryPoint
                                                            );
    } else {
      //
      // Use PE32+ offset
      //
      ImageContext->EntryPoint = (PHYSICAL_ADDRESS)(UINTN)PeCoffLoaderImageAddress (
                                                            ImageContext,
                                                            (UINTN)Hdr.Pe32Plus->OptionalHeader.AddressOfEntryPoint
                                                            );
    }
  } else {
    ImageContext->EntryPoint =  (PHYSICAL_ADDRESS) (
                                (UINTN)ImageContext->ImageAddress  +
                                (UINTN)Hdr.Te->AddressOfEntryPoint +
                                (UINTN)sizeof(EFI_TE_IMAGE_HEADER) -
                                (UINTN)Hdr.Te->StrippedSize
                                );
  }

  //
  // Determine the size of the fixup data
  //
  // Per the PE/COFF spec, you can't assume that a given data directory
  // is present in the image. You have to check the NumberOfRvaAndSizes in
  // the optional header to verify a desired directory entry is there.
  //
  if (!(ImageContext->IsTeImage)) {
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      NumberOfRvaAndSizes = Hdr.Pe32->OptionalHeader.NumberOfRvaAndSizes;
      DirectoryEntry = (EFI_IMAGE_DATA_DIRECTORY *)&Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC];
    } else {
      //
      // Use PE32+ offset
      //
      NumberOfRvaAndSizes = Hdr.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
      DirectoryEntry = (EFI_IMAGE_DATA_DIRECTORY *)&Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC];
    }

    if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC) {
      ImageContext->FixupDataSize = DirectoryEntry->Size / sizeof (UINT16) * sizeof (UINTN);
    } else {
      ImageContext->FixupDataSize = 0;
    }
  } else {
    DirectoryEntry              = &Hdr.Te->DataDirectory[0];
    ImageContext->FixupDataSize = DirectoryEntry->Size / sizeof (UINT16) * sizeof (UINTN);
  }
  //
  // Consumer must allocate a buffer for the relocation fixup log.
  // Only used for runtime drivers.
  //
  ImageContext->FixupData = NULL;

  //
  // Load the Codeview info if present
  //
  if (ImageContext->DebugDirectoryEntryRva != 0) {
    if (!(ImageContext->IsTeImage)) {
      DebugEntry = PeCoffLoaderImageAddress (
                    ImageContext,
                    ImageContext->DebugDirectoryEntryRva
                    );
    } else {
      DebugEntry = (EFI_IMAGE_DEBUG_DIRECTORY_ENTRY *)(UINTN)(
                      ImageContext->ImageAddress +
                      ImageContext->DebugDirectoryEntryRva +
                      sizeof(EFI_TE_IMAGE_HEADER) -
                      Hdr.Te->StrippedSize
                      );
    }

    if (DebugEntry != NULL) {
      TempDebugEntryRva = DebugEntry->RVA;
      if (DebugEntry->RVA == 0 && DebugEntry->FileOffset != 0) {
        Section--;
        if ((UINTN)Section->SizeOfRawData < Section->Misc.VirtualSize) {
          TempDebugEntryRva = Section->VirtualAddress + Section->Misc.VirtualSize;
        } else {
          TempDebugEntryRva = Section->VirtualAddress + Section->SizeOfRawData;
        }
      }

      if (TempDebugEntryRva != 0) {
        if (!(ImageContext->IsTeImage)) {
          ImageContext->CodeView = PeCoffLoaderImageAddress (ImageContext, TempDebugEntryRva);
        } else {
          ImageContext->CodeView = (VOID *)(
                                    (UINTN)ImageContext->ImageAddress +
                                    (UINTN)TempDebugEntryRva +
                                    (UINTN)sizeof (EFI_TE_IMAGE_HEADER) -
                                    (UINTN) Hdr.Te->StrippedSize
                                    );
        }

        if (ImageContext->CodeView == NULL) {
          ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
          return RETURN_LOAD_ERROR;
        }

        if (DebugEntry->RVA == 0) {
          Size = DebugEntry->SizeOfData;
          if (!(ImageContext->IsTeImage)) {
            Status = ImageContext->ImageRead (
                                    ImageContext->Handle,
                                    DebugEntry->FileOffset,
                                    &Size,
                                    ImageContext->CodeView
                                    );
          } else {
            Status = ImageContext->ImageRead (
                                    ImageContext->Handle,
                                    DebugEntry->FileOffset + sizeof (EFI_TE_IMAGE_HEADER) - Hdr.Te->StrippedSize,
                                    &Size,
                                    ImageContext->CodeView
                                    );
            //
            // Should we apply fix up to this field according to the size difference between PE and TE?
            // Because now we maintain TE header fields unfixed, this field will also remain as they are
            // in original PE image.
            //
          }

          if (RETURN_ERROR (Status)) {
            ImageContext->ImageError = IMAGE_ERROR_IMAGE_READ;
            return RETURN_LOAD_ERROR;
          }

          DebugEntry->RVA = TempDebugEntryRva;
        }

        switch (*(UINT32 *) ImageContext->CodeView) {
        case CODEVIEW_SIGNATURE_NB10:
          ImageContext->PdbPointer = (CHAR8 *)ImageContext->CodeView + sizeof (EFI_IMAGE_DEBUG_CODEVIEW_NB10_ENTRY);
          break;

        case CODEVIEW_SIGNATURE_RSDS:
          ImageContext->PdbPointer = (CHAR8 *)ImageContext->CodeView + sizeof (EFI_IMAGE_DEBUG_CODEVIEW_RSDS_ENTRY);
          break;

        default:
          break;
        }
      }
    }
  }

  return Status;
}


/**
  Reapply fixups on a fixed up PE32/PE32+ image to allow virutal calling at EFI
  runtime.

  PE_COFF_LOADER_IMAGE_CONTEXT.FixupData stores information needed to reapply
  the fixups with a virtual mapping.


  @param  ImageBase          Base address of relocated image
  @param  VirtImageBase      Virtual mapping for ImageBase
  @param  ImageSize          Size of the image to relocate
  @param  RelocationData     Location to place results of read

**/
VOID
EFIAPI
PeCoffLoaderRelocateImageForRuntime (
  IN  PHYSICAL_ADDRESS        ImageBase,
  IN  PHYSICAL_ADDRESS        VirtImageBase,
  IN  UINTN                   ImageSize,
  IN  VOID                    *RelocationData
  )
{
  CHAR8                               *OldBase;
  CHAR8                               *NewBase;
  EFI_IMAGE_DOS_HEADER                *DosHdr;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION Hdr;
  UINT32                              NumberOfRvaAndSizes;
  EFI_IMAGE_DATA_DIRECTORY            *DataDirectory;
  EFI_IMAGE_DATA_DIRECTORY            *RelocDir;
  EFI_IMAGE_BASE_RELOCATION           *RelocBase;
  EFI_IMAGE_BASE_RELOCATION           *RelocBaseEnd;
  UINT16                              *Reloc;
  UINT16                              *RelocEnd;
  CHAR8                               *Fixup;
  CHAR8                               *FixupBase;
  UINT16                              *F16;
  UINT32                              *F32;
  UINT64                              *F64;
  CHAR8                               *FixupData;
  UINTN                               Adjust;
  RETURN_STATUS                       Status;
  UINT16                              Magic;

  OldBase = (CHAR8 *)((UINTN)ImageBase);
  NewBase = (CHAR8 *)((UINTN)VirtImageBase);
  Adjust = (UINTN) NewBase - (UINTN) OldBase;

  //
  // Find the image's relocate dir info
  //
  DosHdr = (EFI_IMAGE_DOS_HEADER *)OldBase;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // Valid DOS header so get address of PE header
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)(((CHAR8 *)DosHdr) + DosHdr->e_lfanew);
  } else {
    //
    // No Dos header so assume image starts with PE header.
    //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)OldBase;
  }

  if (Hdr.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    //
    // Not a valid PE image so Exit
    //
    return ;
  }

  Magic = PeCoffLoaderGetPeHeaderMagicValue (Hdr);

  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // Use PE32 offset
    //
    NumberOfRvaAndSizes = Hdr.Pe32->OptionalHeader.NumberOfRvaAndSizes;
    DataDirectory = (EFI_IMAGE_DATA_DIRECTORY *)&(Hdr.Pe32->OptionalHeader.DataDirectory[0]);
  } else {
    //
    // Use PE32+ offset
    //
    NumberOfRvaAndSizes = Hdr.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
    DataDirectory = (EFI_IMAGE_DATA_DIRECTORY *)&(Hdr.Pe32Plus->OptionalHeader.DataDirectory[0]);
  }

  //
  // Find the relocation block
  //
  // Per the PE/COFF spec, you can't assume that a given data directory
  // is present in the image. You have to check the NumberOfRvaAndSizes in
  // the optional header to verify a desired directory entry is there.
  //
  if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC) {
    RelocDir      = DataDirectory + EFI_IMAGE_DIRECTORY_ENTRY_BASERELOC;
    RelocBase     = (EFI_IMAGE_BASE_RELOCATION *)(UINTN)(ImageBase + RelocDir->VirtualAddress);
    RelocBaseEnd  = (EFI_IMAGE_BASE_RELOCATION *)(UINTN)(ImageBase + RelocDir->VirtualAddress + RelocDir->Size);
  } else {
    //
    // Cannot find relocations, cannot continue
    //
    ASSERT (FALSE);
    return ;
  }

  ASSERT (RelocBase != NULL && RelocBaseEnd != NULL);

  //
  // Run the whole relocation block. And re-fixup data that has not been
  // modified. The FixupData is used to see if the image has been modified
  // since it was relocated. This is so data sections that have been updated
  // by code will not be fixed up, since that would set them back to
  // defaults.
  //
  FixupData = RelocationData;
  while (RelocBase < RelocBaseEnd) {

    Reloc     = (UINT16 *) ((UINT8 *) RelocBase + sizeof (EFI_IMAGE_BASE_RELOCATION));
    RelocEnd  = (UINT16 *) ((UINT8 *) RelocBase + RelocBase->SizeOfBlock);
    FixupBase = (CHAR8 *) ((UINTN)ImageBase) + RelocBase->VirtualAddress;

    //
    // Run this relocation record
    //
    while (Reloc < RelocEnd) {

      Fixup = FixupBase + (*Reloc & 0xFFF);
      switch ((*Reloc) >> 12) {

      case EFI_IMAGE_REL_BASED_ABSOLUTE:
        break;

      case EFI_IMAGE_REL_BASED_HIGH:
        F16 = (UINT16 *) Fixup;
        if (*(UINT16 *) FixupData == *F16) {
          *F16 = (UINT16) (*F16 + ((UINT16) ((UINT32) Adjust >> 16)));
        }

        FixupData = FixupData + sizeof (UINT16);
        break;

      case EFI_IMAGE_REL_BASED_LOW:
        F16 = (UINT16 *) Fixup;
        if (*(UINT16 *) FixupData == *F16) {
          *F16 = (UINT16) (*F16 + ((UINT16) Adjust & 0xffff));
        }

        FixupData = FixupData + sizeof (UINT16);
        break;

      case EFI_IMAGE_REL_BASED_HIGHLOW:
        F32       = (UINT32 *) Fixup;
        FixupData = ALIGN_POINTER (FixupData, sizeof (UINT32));
        if (*(UINT32 *) FixupData == *F32) {
          *F32 = *F32 + (UINT32) Adjust;
        }

        FixupData = FixupData + sizeof (UINT32);
        break;

      case EFI_IMAGE_REL_BASED_DIR64:
        F64       = (UINT64 *)Fixup;
        FixupData = ALIGN_POINTER (FixupData, sizeof (UINT64));
        if (*(UINT64 *) FixupData == *F64) {
          *F64 = *F64 + (UINT64)Adjust;
        }

        FixupData = FixupData + sizeof (UINT64);
        break;

      case EFI_IMAGE_REL_BASED_HIGHADJ:
        //
        // Not implemented, but not used in EFI 1.0
        //
        ASSERT (FALSE);
        break;

      default:
        //
        // Only Itanium requires ConvertPeImage_Ex
        //
        Status = PeHotRelocateImageEx (Reloc, Fixup, &FixupData, Adjust);
        if (RETURN_ERROR (Status)) {
          return ;
        }
      }
      //
      // Next relocation record
      //
      Reloc += 1;
    }
    //
    // next reloc block
    //
    RelocBase = (EFI_IMAGE_BASE_RELOCATION *) RelocEnd;
  }
}


/**
  ImageRead function that operates on a memory buffer whos base is passed into
  FileHandle.

  @param  FileHandle        Ponter to baes of the input stream
  @param  FileOffset        Offset to the start of the buffer
  @param  ReadSize          Number of bytes to copy into the buffer
  @param  Buffer            Location to place results of read

  @retval RETURN_SUCCESS    Data is read from FileOffset from the Handle into
                            the buffer.
**/
RETURN_STATUS
EFIAPI
PeCoffLoaderImageReadFromMemory (
  IN     VOID    *FileHandle,
  IN     UINTN   FileOffset,
  IN OUT UINTN   *ReadSize,
  OUT    VOID    *Buffer
  )
{
  CopyMem (Buffer, ((UINT8 *)FileHandle) + FileOffset, *ReadSize);
  return RETURN_SUCCESS;
}

