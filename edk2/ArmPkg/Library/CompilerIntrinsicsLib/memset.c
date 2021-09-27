//------------------------------------------------------------------------------
//
// Copyright (c) 2016, Linaro Ltd. All rights reserved.<BR>
//
// This program and the accompanying materials are licensed and made
// available under the terms and conditions of the BSD License which
// accompanies this distribution.  The full text of the license may be
// found at http://opensource.org/licenses/bsd-license.php
//
// THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
// WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
// IMPLIED.
//
//------------------------------------------------------------------------------

typedef __SIZE_TYPE__ size_t;

static __attribute__((__used__))
void *__memset(void *s, int c, size_t n)
{
  unsigned char *d = s;

  while (n--)
    *d++ = c;

  return s;
}

//
// Other modules (such as CryptoPkg/IntrinsicLib) may provide another
// implementation of memset(), which may conflict with this one if this
// object was pulled into the link due to the definitions below. So make
// our memset() 'weak' to let the other implementation take precedence.
//
__attribute__((__weak__, __alias__("__memset")))
void *memset(void *dest, int c, size_t n);

#ifdef __arm__

void __aeabi_memset(void *dest, size_t n, int c)
{
  __memset(dest, c, n);
}

__attribute__((__alias__("__aeabi_memset")))
void __aeabi_memset4(void *dest, size_t n, int c);

__attribute__((__alias__("__aeabi_memset")))
void __aeabi_memset8(void *dest, size_t n, int c);

void __aeabi_memclr(void *dest, size_t n)
{
  __memset(dest, 0, n);
}

__attribute__((__alias__("__aeabi_memclr")))
void __aeabi_memclr4(void *dest, size_t n);

__attribute__((__alias__("__aeabi_memclr")))
void __aeabi_memclr8(void *dest, size_t n);

#endif
