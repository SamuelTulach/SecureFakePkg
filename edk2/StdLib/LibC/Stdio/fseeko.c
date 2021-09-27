/*
    Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>
    This program and the accompanying materials are licensed and made available
    under the terms and conditions of the BSD License that accompanies this
    distribution.  The full text of the license may be found at
    http://opensource.org/licenses/bsd-license.

    THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
    WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
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

    NetBSD: fseeko.c,v 1.5 2005/03/04 16:04:58 dsl Exp
 */
//#include <Uefi.h>               // REMOVE, For DEBUG only
//#include <Library/UefiLib.h>    // REMOVE, For DEBUG only

#include  <LibConfig.h>
#include <sys/EfiCdefs.h>

#include "namespace.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "reentrant.h"
#include "local.h"

#ifdef __weak_alias
__weak_alias(fseeko, _fseeko)
#endif

#define POS_ERR (-(fpos_t)1)

/*
 * Seek the given file to the given offset.
 * `Whence' must be one of the three SEEK_* macros.
 */
int
fseeko(FILE *fp, off_t offset, int whence)
{
  fpos_t (*seekfn)(void *, fpos_t, int);
  fpos_t target, curoff;
  size_t n;
  struct stat st;
  int havepos;

  _DIAGASSERT(fp != NULL);
  if(fp == NULL) {
    errno = EINVAL;
    return -1;
  }

#ifdef __GNUC__
  /* This outrageous construct just to shut up a GCC warning. */
  (void) &curoff;
#endif

  /* make sure stdio is set up */
  if (!__sdidinit)
    __sinit();

//Print(L"%a( %d, %Ld, %d)\n", __func__, fp->_file, offset, whence);
  FLOCKFILE(fp);

  /*
   * Have to be able to seek.
   */
  if ((seekfn = fp->_seek) == NULL) {
    errno = ESPIPE;     /* historic practice */
    FUNLOCKFILE(fp);
//Print(L"%a: %d\n", __func__, __LINE__);
    return (-1);
  }

  /*
   * Change any SEEK_CUR to SEEK_SET, and check `whence' argument.
   * After this, whence is either SEEK_SET or SEEK_END.
   */
  switch (whence) {

  case SEEK_CUR:
    /*
     * In order to seek relative to the current stream offset,
     * we have to first find the current stream offset a la
     * ftell (see ftell for details).
     */
    __sflush(fp); /* may adjust seek offset on append stream */
    if (fp->_flags & __SOFF)
      curoff = fp->_offset;
    else {
      curoff = (*seekfn)(fp->_cookie, (fpos_t)0, SEEK_CUR);
      if (curoff == POS_ERR) {
        FUNLOCKFILE(fp);
//Print(L"%a: %d\n", __func__, __LINE__);
        return (-1);
      }
    }
    if (fp->_flags & __SRD) {
      curoff -= fp->_r;
      if (HASUB(fp))
        curoff -= fp->_ur;
    } else if (fp->_flags & __SWR && fp->_p != NULL)
      curoff += fp->_p - fp->_bf._base;

    offset += curoff;
    whence = SEEK_SET;
    havepos = 1;
    break;

  case SEEK_SET:
  case SEEK_END:
    curoff = 0;   /* XXX just to keep gcc quiet */
    havepos = 0;
    break;

  default:
    errno = EINVAL;
    FUNLOCKFILE(fp);
//Print(L"%a: %d\n", __func__, __LINE__);
    return (-1);
  }

  /*
   * Can only optimise if:
   *  reading (and not reading-and-writing);
   *  not unbuffered; and
   *  this is a `regular' Unix file (and hence seekfn==__sseek).
   * We must check __NBF first, because it is possible to have __NBF
   * and __SOPT both set.
   */
  if (fp->_bf._base == NULL)
    __smakebuf(fp);
  if (fp->_flags & (__SWR | __SRW | __SNBF | __SNPT))
    goto dumb;
  if ((fp->_flags & __SOPT) == 0) {
    if (seekfn != __sseek ||
        fp->_file < 0 || fstat(fp->_file, &st) ||
        !S_ISREG(st.st_mode)) {
      fp->_flags |= __SNPT;
      goto dumb;
    }
    fp->_blksize = st.st_blksize;
    fp->_flags |= __SOPT;
  }

  /*
   * We are reading; we can try to optimise.
   * Figure out where we are going and where we are now.
   */
  if (whence == SEEK_SET)
    target = offset;
  else {
    if (fstat(fp->_file, &st))
    {
//Print(L"%a: %d\n", __func__, __LINE__);
      goto dumb;
    }
    target = st.st_size + offset;
  }

  if (!havepos) {
    if (fp->_flags & __SOFF)
      curoff = fp->_offset;
    else {
      curoff = (*seekfn)(fp->_cookie, (fpos_t)0, SEEK_CUR);
      if (curoff == POS_ERR)
      {
//Print(L"%a: %d\n", __func__, __LINE__);
        goto dumb;
      }
    }
    curoff -= fp->_r;
    if (HASUB(fp))
      curoff -= fp->_ur;
  }

  /*
   * Compute the number of bytes in the input buffer (pretending
   * that any ungetc() input has been discarded).  Adjust current
   * offset backwards by this count so that it represents the
   * file offset for the first byte in the current input buffer.
   */
  if (HASUB(fp)) {
    curoff += fp->_r; /* kill off ungetc */
    n = fp->_up - fp->_bf._base;
    curoff -= n;
    n += fp->_ur;
  } else {
    n = fp->_p - fp->_bf._base;
    curoff -= n;
    n += fp->_r;
  }

  /*
   * If the target offset is within the current buffer,
   * simply adjust the pointers, clear EOF, undo ungetc(),
   * and return.  (If the buffer was modified, we have to
   * skip this; see fgetln.c.)
   */
  if ((fp->_flags & __SMOD) == 0 &&
      target >= curoff && target < (fpos_t)(curoff + n)) {
    int o = (int)(target - curoff);

    fp->_p = fp->_bf._base + o;
    fp->_r = (int)(n - o);
    if (HASUB(fp))
      FREEUB(fp);
    WCIO_FREE(fp);    /* Should this really be unconditional??? */
    fp->_flags &= ~__SEOF;
    FUNLOCKFILE(fp);
    return (0);
  }

  /*
   * The place we want to get to is not within the current buffer,
   * but we can still be kind to the kernel copyout mechanism.
   * By aligning the file offset to a block boundary, we can let
   * the kernel use the VM hardware to map pages instead of
   * copying bytes laboriously.  Using a block boundary also
   * ensures that we only read one block, rather than two.
   */
  curoff = target & ~(fp->_blksize - 1);
  if ((*seekfn)(fp->_cookie, curoff, SEEK_SET) == POS_ERR)
  {
//Print(L"%a: %d\n", __func__, __LINE__);
    goto dumb;
  }
  fp->_r = 0;
  fp->_p = fp->_bf._base;
  if (HASUB(fp))
    FREEUB(fp);
  WCIO_FREE(fp);    /* Should this really be unconditional??? */
  fp->_flags &= ~__SEOF;
  n = (int)(target - curoff);
  if (n) {
    if (__srefill(fp) || fp->_r < (int)n)
    {
//Print(L"%a: %d\n", __func__, __LINE__);
      goto dumb;
    }
    fp->_p += n;
    fp->_r -= (int)n;
  }
  FUNLOCKFILE(fp);
  return (0);

  /*
   * We get here if we cannot optimise the seek ... just
   * do it.  Allow the seek function to change fp->_bf._base.
   */
dumb:
//Print(L"%a: %d\n", __func__, __LINE__);
  if (__sflush(fp) ||
      (*seekfn)(fp->_cookie, (fpos_t)offset, whence) == POS_ERR) {
    FUNLOCKFILE(fp);
//Print(L"%a: %d\n", __func__, __LINE__);
    return (-1);
  }
  /* success: clear EOF indicator and discard ungetc() data */
  if (HASUB(fp))
    FREEUB(fp);
  WCIO_FREE(fp);    /* Should this really be unconditional??? */
  fp->_p = fp->_bf._base;
  fp->_r = 0;
  fp->_w = 0;
  fp->_flags &= ~__SEOF;
  FUNLOCKFILE(fp);
//Print(L"%a: %d\n", __func__, __LINE__);
  return (0);
}
