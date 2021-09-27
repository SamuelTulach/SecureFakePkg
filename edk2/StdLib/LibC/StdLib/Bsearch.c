/** @file
  Binary search utility function.

  This utility makes use of a comparison function to search arrays of
  unspecified type. Where an argument declared as size_t nmemb specifies the
  length of the array for a function, nmemb can have the value zero on a call
  to that function; the comparison function is not called, a search finds no
  matching element. Pointer arguments on such a call shall still have valid
  values.

  The implementation shall ensure that the second argument of the comparison
  function is a pointer to an element of the array. The first argument shall
  equal key.

  The comparison function shall not alter the contents of the array. The
  implementation may reorder elements of the array between calls to the
  comparison function, but shall not alter the contents of any individual
  element.

  When the same objects (consisting of size bytes, irrespective of their
  current positions in the array) are passed more than once to the comparison
  function, the results shall be consistent with one another. That is, the same
  object shall always compare the same way with the key.

  A sequence point occurs immediately before and immediately after each call to
  the comparison function, and also between any call to the comparison function
  and any movement of the objects passed as arguments to that call.

  Copyright (c) 2010, Intel Corporation. All rights reserved.<BR>

 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.

  ("$FreeBSD: src/lib/libc/stdlib/bsearch.c,v 1.4.10.1.2.1 2009/10/25 01:10:29 kensmith Exp $");
**/
#include  <LibConfig.h>
#include  <sys/EfiCdefs.h>
#include  <stdlib.h>

/*
 * Perform a binary search.
 *
 * The code below is a bit sneaky.  After a comparison fails, we
 * divide the work in half by moving either left or right. If lim
 * is odd, moving left simply involves halving lim: e.g., when lim
 * is 5 we look at item 2, so we change lim to 2 so that we will
 * look at items 0 & 1.  If lim is even, the same applies.  If lim
 * is odd, moving right again involes halving lim, this time moving
 * the base up one item past p: e.g., when lim is 5 we change base
 * to item 3 and make lim 2 so that we will look at items 3 and 4.
 * If lim is even, however, we have to shrink it by one before
 * halving: e.g., when lim is 4, we still looked at item 2, so we
 * have to make lim 3, then halve, obtaining 1, so that we will only
 * look at item 3.
 */
void *
bsearch(
  const void *key,
  const void *base0,
  size_t nmemb,
  size_t size,
  int (*compar)(const void *, const void *)
  )
{
  const char *base = base0;
  size_t      lim;
  int         cmp;
  const void *p;

  for (lim = nmemb; lim != 0; lim >>= 1) {
    p = base + (lim >> 1) * size;
    cmp = (*compar)(key, p);
    if (cmp == 0)
      return ((void *)p);
    if (cmp > 0) {  /* key > p: move right */
      base = (char *)p + size;
      lim--;
    }   /* else move left */
  }
  return (NULL);
}
