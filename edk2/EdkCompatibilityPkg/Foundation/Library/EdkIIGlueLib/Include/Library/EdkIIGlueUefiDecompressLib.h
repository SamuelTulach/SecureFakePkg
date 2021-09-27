/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  EdkIIGlueUefiDecompressLib.h
  
Abstract: 

  Public header file for UEFI Decompress Lib

--*/

#ifndef __EDKII_GLUE_UEFI_DECPOMPRESS_LIB_H__
#define __EDKII_GLUE_UEFI_DECPOMPRESS_LIB_H__

#define FillBuf(_SD, _NUMOFBITS)                GlueFillBuf(_SD, _NUMOFBITS)
#define GetBits(_SD, _NUMOFBITS)                GlueGetBits(_SD, _NUMOFBITS)
#define MakeTable(_SD, _NUMOFCHAR, _BITLEN, _TABLEBITS, _TABLE) GlueMakeTable(_SD, _NUMOFCHAR, _BITLEN, _TABLEBITS, _TABLE)
#define DecodeP(_SD)                            GlueDecodeP(_SD)
#define ReadPTLen( _SD, _NN, _NBIT, _SPECIAL)   GlueReadPTLen( _SD, _NN, _NBIT, _SPECIAL)
#define ReadCLen(_SD)                           GlueReadCLen(_SD)
#define DecodeC(_SD)                            GlueDecodeC(_SD)
#define Decode(_SD)                             GlueDecode(_SD)



/**
  Retrieves the size of the uncompressed buffer and the size of the scratch buffer.

  Retrieves the size of the uncompressed buffer and the temporary scratch buffer 
  required to decompress the buffer specified by Source and SourceSize.
  If the size of the uncompressed buffer or the size of the scratch buffer cannot
  be determined from the compressed data specified by Source and SourceData, 
  then RETURN_INVALID_PARAMETER is returned.  Otherwise, the size of the uncompressed
  buffer is returned in DestinationSize, the size of the scratch buffer is returned
  in ScratchSize, and RETURN_SUCCESS is returned.
  This function does not have scratch buffer available to perform a thorough 
  checking of the validity of the source data.  It just retrieves the "Original Size"
  field from the beginning bytes of the source data and output it as DestinationSize.
  And ScratchSize is specific to the decompression implementation.

  If Source is NULL, then ASSERT().
  If DestinationSize is NULL, then ASSERT().
  If ScratchSize is NULL, then ASSERT().

  @param  Source          The source buffer containing the compressed data.
  @param  SourceSize      The size, in bytes, of the source buffer.
  @param  DestinationSize A pointer to the size, in bytes, of the uncompressed buffer
                          that will be generated when the compressed buffer specified
                          by Source and SourceSize is decompressed..
  @param  ScratchSize     A pointer to the size, in bytes, of the scratch buffer that
                          is required to decompress the compressed buffer specified 
                          by Source and SourceSize.

  @retval  RETURN_SUCCESS The size of destination buffer and the size of scratch 
                          buffer are successfully retrieved.
  @retval  RETURN_INVALID_PARAMETER The source data is corrupted

**/
RETURN_STATUS
EFIAPI
UefiDecompressGetInfo (
  IN  CONST VOID  *Source,
  IN  UINT32      SourceSize,
  OUT UINT32      *DestinationSize,
  OUT UINT32      *ScratchSize
  );

/**
  Decompresses a compressed source buffer.

  This function is designed so that the decompression algorithm can be implemented
  without using any memory services.  As a result, this function is not allowed to
  call any memory allocation services in its implementation.  It is the caller's r
  esponsibility to allocate and free the Destination and Scratch buffers.
  If the compressed source data specified by Source is sucessfully decompressed 
  into Destination, then RETURN_SUCCESS is returned.  If the compressed source data 
  specified by Source is not in a valid compressed data format,
  then RETURN_INVALID_PARAMETER is returned.

  If Source is NULL, then ASSERT().
  If Destination is NULL, then ASSERT().
  If the required scratch buffer size > 0 and Scratch is NULL, then ASSERT().

  @param  Source      The source buffer containing the compressed data.
  @param  Destination The destination buffer to store the decompressed data
  @param  Scratch     A temporary scratch buffer that is used to perform the decompression.
                      This is an optional parameter that may be NULL if the 
                      required scratch buffer size is 0.
                     
  @retval  RETURN_SUCCESS Decompression is successfull
  @retval  RETURN_INVALID_PARAMETER The source data is corrupted

**/
RETURN_STATUS
EFIAPI
UefiDecompress (
  IN CONST VOID  *Source,
  IN OUT VOID    *Destination,
  IN OUT VOID    *Scratch
  );

#endif
