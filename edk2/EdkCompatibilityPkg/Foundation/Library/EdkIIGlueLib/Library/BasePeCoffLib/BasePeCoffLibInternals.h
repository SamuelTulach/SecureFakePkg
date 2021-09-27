/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  

Module Name:

  BasePeCoffLibInternals.h
  
Abstract: 

  Header file for Base PeCoff Library
  
--*/

#ifndef __BASE_PECOFF_LIB_INTERNALS__
#define __BASE_PECOFF_LIB_INTERNALS__

#include "EdkIIGlueBase.h"

/**
  Performs an Itanium-based specific relocation fixup and is a no-op on other
  instruction sets.

  @param  Reloc       Pointer to the relocation record.
  @param  Fixup       Pointer to the address to fix up.
  @param  FixupData   Pointer to a buffer to log the fixups.
  @param  Adjust      The offset to adjust the fixup.

  @return Status code.

**/
RETURN_STATUS
GluePeCoffLoaderRelocateImageEx (
  IN UINT16      *Reloc,
  IN OUT CHAR8   *Fixup,
  IN OUT CHAR8   **FixupData,
  IN UINT64      Adjust
  );


/**
  Performs an Itanium-based specific re-relocation fixup and is a no-op on other
  instruction sets. This is used to re-relocated the image into the EFI virtual
  space for runtime calls.

  @param  Reloc       Pointer to the relocation record.
  @param  Fixup       Pointer to the address to fix up.
  @param  FixupData   Pointer to a buffer to log the fixups.
  @param  Adjust      The offset to adjust the fixup.

  @return Status code.

**/
RETURN_STATUS
GluePeHotRelocateImageEx (
  IN UINT16      *Reloc,
  IN OUT CHAR8   *Fixup,
  IN OUT CHAR8   **FixupData,
  IN UINT64      Adjust
  );


/**
  Returns TRUE if the machine type of PE/COFF image is supported. Supported
  does not mean the image can be executed it means the PE/COFF loader supports
  loading and relocating of the image type. It's up to the caller to support
  the entry point.

  @param  Machine   Machine type from the PE Header.

  @return TRUE if this PE/COFF loader can load the image

**/
BOOLEAN
PeCoffLoaderImageFormatSupported (
  IN  UINT16  Machine
  );

/**
  Retrieves the magic value from the PE/COFF header.

  @param  Hdr             The buffer in which to return the PE32, PE32+, or TE header.

  @return EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC - Image is PE32
  @return EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC - Image is PE32+

**/
UINT16
PeCoffLoaderGetPeHeaderMagicValue (
  IN  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr
  );

/**
  Retrieves the PE or TE Header from a PE/COFF or TE image.

  @param  ImageContext    The context of the image being loaded.
  @param  Hdr             The buffer in which to return the PE32, PE32+, or TE header.

  @retval RETURN_SUCCESS  The PE or TE Header is read.
  @retval Other           The error status from reading the PE/COFF or TE image using the ImageRead function.

**/
RETURN_STATUS
PeCoffLoaderGetPeHeader (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT         *ImageContext,
  OUT    EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr
  );

/**
  Converts an image address to the loaded address.

  @param  ImageContext  The context of the image being loaded.
  @param  Address       The address to be converted to the loaded address.

  @return The converted address or NULL if the address can not be converted.

**/
VOID *
PeCoffLoaderImageAddress (
  IN OUT PE_COFF_LOADER_IMAGE_CONTEXT          *ImageContext,
  IN     UINTN                                 Address
  );

#endif
