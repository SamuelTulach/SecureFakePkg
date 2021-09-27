/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.              


Module Name:

  MemoryAllocationLib.c
  
Abstract: 

  Support routines for memory allocation routines for use with drivers.

--*/

#include "EdkIIGluePeim.h"

/**
  Allocates one or more 4KB pages of a certain memory type.

  Allocates the number of 4KB pages of a certain memory type and returns a pointer to the allocated
  buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL is returned.
  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  MemoryType            The type of memory to allocate.
  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocatePages (
  IN EFI_MEMORY_TYPE  MemoryType,  
  IN UINTN            Pages
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory; 

  if (Pages == 0) {
    return NULL;
  }

  Status = PeiServicesAllocatePages (MemoryType, Pages, &Memory);
  if (EFI_ERROR (Status)) {
    Memory = 0;
  }
  return (VOID *) (UINTN) Memory;
}

/**
  Allocates one or more 4KB pages of type EfiBootServicesData.

  Allocates the number of 4KB pages of type EfiBootServicesData and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
GlueAllocatePages (
  IN UINTN  Pages
  )
{
  return InternalAllocatePages (EfiBootServicesData, Pages);
}

/**
  Allocates one or more 4KB pages of type EfiRuntimeServicesData.

  Allocates the number of 4KB pages of type EfiRuntimeServicesData and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateRuntimePages (
  IN UINTN  Pages
  )
{
  return InternalAllocatePages (EfiRuntimeServicesData, Pages);
}

/**
  Allocates one or more 4KB pages of type EfiReservedMemoryType.

  Allocates the number of 4KB pages of type EfiReservedMemoryType and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateReservedPages (
  IN UINTN  Pages
  )
{
  return InternalAllocatePages (EfiReservedMemoryType, Pages);
}

/**
  Frees one or more 4KB pages that were previously allocated with one of the page allocation
  functions in the Memory Allocation Library.

  Frees the number of 4KB pages specified by Pages from the buffer specified by Buffer.  Buffer
  must have been allocated on a previous call to the page allocation services of the Memory
  Allocation Library.
  If Buffer was not allocated with a page allocation function in the Memory Allocation Library,
  then ASSERT().
  If Pages is zero, then ASSERT().
 
  @param  Buffer                Pointer to the buffer of pages to free.
  @param  Pages                 The number of 4 KB pages to free.

**/
VOID
EFIAPI
GlueFreePages (
  IN VOID   *Buffer,
  IN UINTN  Pages
  )
{
  //
  // PEI phase does not support to free pages, so leave it as NOP.
  //
}

/**
  Allocates one or more 4KB pages of a certain memory type at a specified alignment.

  Allocates the number of 4KB pages specified by Pages of a certain memory type with an alignment
  specified by Alignment.  The allocated buffer is returned.  If Pages is 0, then NULL is returned.
  If there is not enough memory at the specified alignment remaining to satisfy the request, then
  NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  MemoryType            The type of memory to allocate.
  @param  Pages                 The number of 4 KB pages to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateAlignedPages (
  IN EFI_MEMORY_TYPE  MemoryType,  
  IN UINTN            Pages,
  IN UINTN            Alignment
  )
{
  VOID    *Memory;
  UINTN   AlignmentMask;

  //
  // Alignment must be a power of two or zero.
  //
  ASSERT ((Alignment & (Alignment - 1)) == 0);

  if (Pages == 0) {
    return NULL;
  }
  //
  // Make sure that Pages plus EFI_SIZE_TO_PAGES (Alignment) does not overflow.
  //
  ASSERT (Pages <= (MAX_ADDRESS - EFI_SIZE_TO_PAGES (Alignment)));
  //
  // We would rather waste some memory to save PEI code size.
  //
  Memory = InternalAllocatePages (MemoryType, Pages + EFI_SIZE_TO_PAGES (Alignment));
  if (Alignment == 0) {
    AlignmentMask = Alignment;
  } else {
    AlignmentMask = Alignment - 1;  
  }
  return (VOID *) (UINTN) (((UINTN) Memory + AlignmentMask) & ~AlignmentMask);
}

/**
  Allocates one or more 4KB pages of type EfiBootServicesData at a specified alignment.

  Allocates the number of 4KB pages specified by Pages of type EfiBootServicesData with an
  alignment specified by Alignment.  The allocated buffer is returned.  If Pages is 0, then NULL is
  returned.  If there is not enough memory at the specified alignment remaining to satisfy the
  request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  Pages                 The number of 4 KB pages to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedPages (
  IN UINTN  Pages,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedPages (EfiBootServicesData, Pages, Alignment);
}

/**
  Allocates one or more 4KB pages of type EfiRuntimeServicesData at a specified alignment.

  Allocates the number of 4KB pages specified by Pages of type EfiRuntimeServicesData with an
  alignment specified by Alignment.  The allocated buffer is returned.  If Pages is 0, then NULL is
  returned.  If there is not enough memory at the specified alignment remaining to satisfy the
  request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  Pages                 The number of 4 KB pages to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedRuntimePages (
  IN UINTN  Pages,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedPages (EfiRuntimeServicesData, Pages, Alignment);
}

/**
  Allocates one or more 4KB pages of type EfiReservedMemoryType at a specified alignment.

  Allocates the number of 4KB pages specified by Pages of type EfiReservedMemoryType with an
  alignment specified by Alignment.  The allocated buffer is returned.  If Pages is 0, then NULL is
  returned.  If there is not enough memory at the specified alignment remaining to satisfy the
  request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  Pages                 The number of 4 KB pages to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedReservedPages (
  IN UINTN  Pages,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedPages (EfiReservedMemoryType, Pages, Alignment);
}

/**
  Frees one or more 4KB pages that were previously allocated with one of the aligned page
  allocation functions in the Memory Allocation Library.

  Frees the number of 4KB pages specified by Pages from the buffer specified by Buffer.  Buffer
  must have been allocated on a previous call to the aligned page allocation services of the Memory
  Allocation Library.
  If Buffer was not allocated with an aligned page allocation function in the Memory Allocation
  Library, then ASSERT().
  If Pages is zero, then ASSERT().
  
  @param  Buffer                Pointer to the buffer of pages to free.
  @param  Pages                 The number of 4 KB pages to free.

**/
VOID
EFIAPI
FreeAlignedPages (
  IN VOID   *Buffer,
  IN UINTN  Pages
  )
{
  //
  // PEI phase does not support to free pages, so leave it as NOP.
  //
}

/**
  Allocates a buffer of a certain pool type.

  Allocates the number bytes specified by AllocationSize of a certain pool type and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  MemoryType            The type of memory to allocate.
  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocatePool (
  IN EFI_MEMORY_TYPE  MemoryType,  
  IN UINTN            AllocationSize
  )
{
  //
  // If we need lots of small runtime/reserved memory type from PEI in the future, 
  // we can consider providing a more complex algorithm that allocates runtime pages and 
  // provide pool allocations from those pages. 
  //
  return InternalAllocatePages (MemoryType, EFI_SIZE_TO_PAGES (AllocationSize));
}

/**
  Allocates a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData and returns a
  pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
GlueAllocatePool (
  IN UINTN  AllocationSize
  )
{
  EFI_STATUS        Status;
  VOID              *Buffer;
  
  Status = PeiServicesAllocatePool (AllocationSize, &Buffer);
  if (EFI_ERROR (Status)) {
    Buffer = NULL;
  }
  return Buffer;
}

/**
  Allocates a buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData and returns
  a pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateRuntimePool (
  IN UINTN  AllocationSize
  )
{
  return InternalAllocatePool (EfiRuntimeServicesData, AllocationSize);
}

/**
  Allocates a buffer of type EfieservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfieservedMemoryType and returns
  a pointer to the allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is
  returned.  If there is not enough memory remaining to satisfy the request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateReservedPool (
  IN UINTN  AllocationSize
  )
{
  return InternalAllocatePool (EfiReservedMemoryType, AllocationSize);
}

/**
  Allocates and zeros a buffer of a certian pool type.

  Allocates the number bytes specified by AllocationSize of a certian pool type, clears the buffer
  with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a valid
  buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the request,
  then NULL is returned.

  @param  PoolType              The type of memory to allocate.
  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateZeroPool (
  IN EFI_MEMORY_TYPE  PoolType,  
  IN UINTN            AllocationSize
  ) 
{
  VOID  *Memory;

  Memory = InternalAllocatePool (PoolType, AllocationSize);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}

/**
  Allocates and zeros a buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
GlueAllocateZeroPool (
  IN UINTN  AllocationSize
  )
{
  VOID  *Memory;

  Memory = AllocatePool (AllocationSize);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}

/**
  Allocates and zeros a buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateRuntimeZeroPool (
  IN UINTN  AllocationSize
  )
{
  return InternalAllocateZeroPool (EfiRuntimeServicesData, AllocationSize);
}

/**
  Allocates and zeros a buffer of type EfiReservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType, clears the
  buffer with zeros, and returns a pointer to the allocated buffer.  If AllocationSize is 0, then a
  valid buffer of 0 size is returned.  If there is not enough memory remaining to satisfy the
  request, then NULL is returned.

  @param  AllocationSize        The number of bytes to allocate and zero.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateReservedZeroPool (
  IN UINTN  AllocationSize
  )
{
  return InternalAllocateZeroPool (EfiReservedMemoryType, AllocationSize);
}

/**
  Copies a buffer to an allocated buffer of a certian pool type.

  Allocates the number bytes specified by AllocationSize of a certian pool type, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS ? Buffer + 1), then ASSERT(). 

  @param  PoolType              The type of pool to allocate.
  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateCopyPool (
  IN EFI_MEMORY_TYPE  PoolType,  
  IN UINTN            AllocationSize,
  IN CONST VOID       *Buffer
  ) 
{
  VOID  *Memory;

  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = InternalAllocatePool (PoolType, AllocationSize);
  if (Memory != NULL) {
     Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
} 

/**
  Copies a buffer to an allocated buffer of type EfiBootServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS ? Buffer + 1), then ASSERT(). 

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
GlueAllocateCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  )
{
  VOID  *Memory;

  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = AllocatePool (AllocationSize);
  if (Memory != NULL) {
     Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
}

/**
  Copies a buffer to an allocated buffer of type EfiRuntimeServicesData.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS ? Buffer + 1), then ASSERT(). 

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateRuntimeCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  )
{
  return InternalAllocateCopyPool (EfiRuntimeServicesData, AllocationSize, Buffer);
}

/**
  Copies a buffer to an allocated buffer of type EfiReservedMemoryType.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType, copies
  AllocationSize bytes from Buffer to the newly allocated buffer, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory remaining to satisfy the request, then NULL is returned.
  If Buffer is NULL, then ASSERT().
  If AllocationSize is greater than (MAX_ADDRESS ? Buffer + 1), then ASSERT(). 

  @param  AllocationSize        The number of bytes to allocate and zero.
  @param  Buffer                The buffer to copy to the allocated buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateReservedCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer
  )
{
  return InternalAllocateCopyPool (EfiReservedMemoryType, AllocationSize, Buffer);
}

/**
  Frees a buffer that was previously allocated with one of the pool allocation functions in the
  Memory Allocation Library.

  Frees the buffer specified by Buffer.  Buffer must have been allocated on a previous call to the
  pool allocation services of the Memory Allocation Library.
  If Buffer was not allocated with a pool allocation function in the Memory Allocation Library,
  then ASSERT().

  @param  Buffer                Pointer to the buffer to free.

**/
VOID
EFIAPI
GlueFreePool (
  IN VOID   *Buffer
  )
{
  //
  // PEI phase does not support to free pool, so leave it as NOP.
  //
}

/**
  Allocates a buffer of a certain pool type at a specified alignment.

  Allocates the number bytes specified by AllocationSize of a certain pool type with an alignment
  specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0, then a valid
  buffer of 0 size is returned.  If there is not enough memory at the specified alignment remaining
  to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  PoolType              The type of pool to allocate.
  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.                            If Alignment is zero, then byte alignment is used.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateAlignedPool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            AllocationSize,
  IN UINTN            Alignment
  )
{
  VOID    *RawAddress;
  UINTN   AlignedAddress;
  UINTN   AlignmentMask;

  //
  // Alignment must be a power of two or zero.
  //
  ASSERT ((Alignment & (Alignment - 1)) == 0);
  
  if (Alignment == 0) {
    AlignmentMask = Alignment;
  } else {
    AlignmentMask = Alignment - 1;
  }
  //
  // Make sure that AllocationSize plus AlignmentMask does not overflow.
  //
  ASSERT (AllocationSize <= (MAX_ADDRESS - AlignmentMask));

  RawAddress      = InternalAllocatePool (PoolType, AllocationSize + AlignmentMask);
  
  AlignedAddress  = ((UINTN) RawAddress + AlignmentMask) & ~AlignmentMask;
    
  return (VOID *) AlignedAddress;
}

/**
  Allocates a buffer of type EfiBootServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedPool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  VOID    *RawAddress;
  UINTN   AlignedAddress;
  UINTN   AlignmentMask;

  //
  // Alignment must be a power of two or zero.
  //
  ASSERT ((Alignment & (Alignment - 1)) == 0);

  if (Alignment == 0) {
    AlignmentMask = Alignment;
  } else {
    AlignmentMask = Alignment - 1;
  }

  //
  // Make sure that AllocationSize plus AlignmentMask does not overflow.
  //
  ASSERT (AllocationSize <= (MAX_ADDRESS - AlignmentMask));

  RawAddress      = AllocatePool (AllocationSize + AlignmentMask);
  
  AlignedAddress  = ((UINTN) RawAddress + AlignmentMask) & ~AlignmentMask;
    
  return (VOID *) AlignedAddress;
}

/**
  Allocates a buffer of type EfiRuntimeServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedRuntimePool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedPool (EfiRuntimeServicesData, AllocationSize, Alignment);
}

/**
  Allocates a buffer of type EfieservedMemoryType at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfieservedMemoryType with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedReservedPool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedPool (EfiReservedMemoryType, AllocationSize, Alignment);
}

/**
  Allocates and zeros a buffer of a certain pool type at a specified alignment.

  Allocates the number bytes specified by AllocationSize of a certain pool type with an alignment
  specified by Alignment, clears the buffer with zeros, and returns a pointer to the allocated
  buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there is not
  enough memory at the specified alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  PoolType              The type of pool to allocate.
  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateAlignedZeroPool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            AllocationSize,
  IN UINTN            Alignment
  )
{
  VOID    *Memory;

  Memory = InternalAllocateAlignedPool (PoolType, AllocationSize, Alignment);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}

/**
  Allocates and zeros a buffer of type EfiBootServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData with an
  alignment specified by Alignment, clears the buffer with zeros, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory at the specified alignment remaining to satisfy the request, then NULL is
  returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedZeroPool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  VOID    *Memory;

  Memory = AllocateAlignedPool (AllocationSize, Alignment);
  if (Memory != NULL) {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}

/**
  Allocates and zeros a buffer of type EfiRuntimeServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData with an
  alignment specified by Alignment, clears the buffer with zeros, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory at the specified alignment remaining to satisfy the request, then NULL is
  returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedRuntimeZeroPool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedZeroPool (EfiRuntimeServicesData, AllocationSize, Alignment);
}

/**
  Allocates and zeros a buffer of type EfieservedMemoryType at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfieservedMemoryType with an
  alignment specified by Alignment, clears the buffer with zeros, and returns a pointer to the
  allocated buffer.  If AllocationSize is 0, then a valid buffer of 0 size is returned.  If there
  is not enough memory at the specified alignment remaining to satisfy the request, then NULL is
  returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedReservedZeroPool (
  IN UINTN  AllocationSize,
  IN UINTN  Alignment
  )
{
  return InternalAllocateAlignedZeroPool (EfiReservedMemoryType, AllocationSize, Alignment);
}

/**
  Copies a buffer to an allocated buffer of a certain pool type at a specified alignment.

  Allocates the number bytes specified by AllocationSize of a certain pool type with an alignment
  specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0, then a valid
  buffer of 0 size is returned.  If there is not enough memory at the specified alignment remaining
  to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  PoolType              The type of pool to allocate.
  @param  AllocationSize        The number of bytes to allocate.
  @param  Buffer                The buffer to copy to the allocated buffer.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
InternalAllocateAlignedCopyPool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            AllocationSize,
  IN CONST VOID       *Buffer,
  IN UINTN            Alignment
  )
{
  VOID  *Memory;
  
  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = InternalAllocateAlignedPool (PoolType, AllocationSize, Alignment);
  if (Memory != NULL) {
    Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
}

/**
  Copies a buffer to an allocated buffer of type EfiBootServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiBootServicesData type with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Buffer                The buffer to copy to the allocated buffer.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer,
  IN UINTN       Alignment
  )
{
  VOID  *Memory;
  
  ASSERT (Buffer != NULL);
  ASSERT (AllocationSize <= (MAX_ADDRESS - (UINTN) Buffer + 1));

  Memory = AllocateAlignedPool (AllocationSize, Alignment);
  if (Memory != NULL) {
    Memory = CopyMem (Memory, Buffer, AllocationSize);
  }
  return Memory;
}

/**
  Copies a buffer to an allocated buffer of type EfiRuntimeServicesData at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiRuntimeServicesData type with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Buffer                The buffer to copy to the allocated buffer.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedRuntimeCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer,
  IN UINTN       Alignment
  )
{
  return InternalAllocateAlignedCopyPool (EfiRuntimeServicesData, AllocationSize, Buffer, Alignment);
}

/**
  Copies a buffer to an allocated buffer of type EfiReservedMemoryType at a specified alignment.

  Allocates the number bytes specified by AllocationSize of type EfiReservedMemoryType type with an
  alignment specified by Alignment.  The allocated buffer is returned.  If AllocationSize is 0,
  then a valid buffer of 0 size is returned.  If there is not enough memory at the specified
  alignment remaining to satisfy the request, then NULL is returned.
  If Alignment is not a power of two and Alignment is not zero, then ASSERT().

  @param  AllocationSize        The number of bytes to allocate.
  @param  Buffer                The buffer to copy to the allocated buffer.
  @param  Alignment             The requested alignment of the allocation.  Must be a power of two.
                                If Alignment is zero, then byte alignment is used.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID *
EFIAPI
AllocateAlignedReservedCopyPool (
  IN UINTN       AllocationSize,
  IN CONST VOID  *Buffer,
  IN UINTN       Alignment
  )
{
  return InternalAllocateAlignedCopyPool (EfiReservedMemoryType, AllocationSize, Buffer, Alignment);
}

/**
  Frees a buffer that was previously allocated with one of the aligned pool allocation functions 
  in the Memory Allocation Library.

  Frees the buffer specified by Buffer.  Buffer must have been allocated on a previous call to the
  aligned pool allocation services of the Memory Allocation Library.
  If Buffer was not allocated with an aligned pool allocation function in the Memory Allocation
  Library, then ASSERT().

  @param  Buffer                Pointer to the buffer to free.

**/
VOID
EFIAPI
FreeAlignedPool (
  IN VOID   *Buffer
  )
{
  //
  // PEI phase does not support to free pool, so leave it as NOP.
  //
}
