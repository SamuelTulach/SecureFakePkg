/*
 * Copyright (c) 1996, 1998 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * Portions copyright (c) 1999, 2000 - 2014
 * Intel Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *
 *    This product includes software developed by Intel Corporation and
 *    its contributors.
 *
 * 4. Neither the name of Intel Corporation or its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY INTEL CORPORATION AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL INTEL CORPORATION OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* Import. */

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>

#include <assert.h>
#include <errno.h>
#include <resolv.h>
#include <string.h>
#include <ctype.h>

#define SPRINTF(x) (sprintf x)

/* Forward. */

static size_t prune_origin(const char *name, const char *origin);
static int  charstr(const u_char *rdata, const u_char *edata,
      char **buf, size_t *buflen);
static int  addname(const u_char *msg, size_t msglen,
      const u_char **p, const char *origin,
      char **buf, size_t *buflen);
static void addlen(size_t len, char **buf, size_t *buflen);
static int  addstr(const char *src, size_t len,
           char **buf, size_t *buflen);
static int  addtab(size_t len, size_t target, int spaced,
           char **buf, size_t *buflen);

/* Macros. */

#define T(x) \
  do { \
    if ((ssize_t)(x) < 0) \
      return (-1); \
  } while (0)

/* Public. */

/*
 * int
 * ns_sprintrr(handle, rr, name_ctx, origin, buf, buflen)
 *  Convert an RR to presentation format.
 * return:
 *  Number of characters written to buf, or -1 (check errno).
 */
int
ns_sprintrr(const ns_msg *handle, const ns_rr *rr,
      const char *name_ctx, const char *origin,
      char *buf, size_t buflen)
{
  int n;

  n = ns_sprintrrf(ns_msg_base(*handle), ns_msg_size(*handle),
       ns_rr_name(*rr), ns_rr_class(*rr), ns_rr_type(*rr),
       ns_rr_ttl(*rr), ns_rr_rdata(*rr), ns_rr_rdlen(*rr),
       name_ctx, origin, buf, buflen);
  return (n);
}

/*
 * int
 * ns_sprintrrf(msg, msglen, name, class, type, ttl, rdata, rdlen,
 *         name_ctx, origin, buf, buflen)
 *  Convert the fields of an RR into presentation format.
 * return:
 *  Number of characters written to buf, or -1 (check errno).
 */
int
ns_sprintrrf(const u_char *msg, size_t msglen,
      const char *name, ns_class class, ns_type type,
      u_long ttl, const u_char *rdata, size_t rdlen,
      const char *name_ctx, const char *origin,
      char *buf, size_t buflen)
{
  const char *obuf = buf;
  const u_char *edata = rdata + rdlen;
  int spaced = 0;

  const char *comment;
  char tmp[100];
  int x;
  size_t len;

  static  char base64_key[NS_MD5RSA_MAX_BASE64];
  static  char t[255*3];

  /*
   * Owner.
   */
  if (name_ctx != NULL && strcasecmp(name_ctx, name) == 0) {
    T(addstr("\t\t\t", 3, &buf, &buflen));
  } else {
    len = prune_origin(name, origin);
    if (len == 0) {
      T(addstr("@\t\t\t", 4, &buf, &buflen));
    } else {
      T(addstr(name, len, &buf, &buflen));
      /* Origin not used and no trailing dot? */
      if ((!origin || !origin[0] || name[len] == '\0') &&
          name[len - 1] != '.') {
        T(addstr(".", 1, &buf, &buflen));
        len++;
      }
      T(spaced = addtab(len, 24, spaced, &buf, &buflen));
    }
  }

  /*
   * TTL, Class, Type.
   */
  T(x = ns_format_ttl(ttl, buf, buflen));
  addlen(x, &buf, &buflen);
  len = SPRINTF((tmp, " %s %s", p_class(class), p_type(type)));
  T(addstr(tmp, len, &buf, &buflen));
  T(spaced = addtab(x + len, 16, spaced, &buf, &buflen));

  /*
   * RData.
   */
  switch (type) {
  case ns_t_a:
    if (rdlen != NS_INADDRSZ)
      goto formerr;
    (void) inet_ntop(AF_INET, rdata, buf, (socklen_t)buflen);
    addlen(strlen(buf), &buf, &buflen);
    break;

  case ns_t_cname:
  case ns_t_mb:
  case ns_t_mg:
  case ns_t_mr:
  case ns_t_ns:
  case ns_t_ptr:
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    break;

  case ns_t_hinfo:
  case ns_t_isdn:
    /* First word. */
    T(len = charstr(rdata, edata, &buf, &buflen));
    if (len == 0)
      goto formerr;
    rdata += len;
    T(addstr(" ", 1, &buf, &buflen));

    /* Second word. */
    T(len = charstr(rdata, edata, &buf, &buflen));
    if (len == 0)
      goto formerr;
    rdata += len;
    break;

  case ns_t_soa: {
    u_long t;

    /* Server name. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    T(addstr(" ", 1, &buf, &buflen));

    /* Administrator name. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    T(addstr(" (\n", 3, &buf, &buflen));
    spaced = 0;

    if ((edata - rdata) != 5*NS_INT32SZ)
      goto formerr;

    /* Serial number. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    T(addstr("\t\t\t\t\t", 5, &buf, &buflen));
    len = SPRINTF((tmp, "%lu", (unsigned long)t));
    T(addstr(tmp, len, &buf, &buflen));
    T(spaced = addtab(len, 16, spaced, &buf, &buflen));
    T(addstr("; serial\n", 9, &buf, &buflen));
    spaced = 0;

    /* Refresh interval. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    T(addstr("\t\t\t\t\t", 5, &buf, &buflen));
    T(len = ns_format_ttl(t, buf, buflen));
    addlen(len, &buf, &buflen);
    T(spaced = addtab(len, 16, spaced, &buf, &buflen));
    T(addstr("; refresh\n", 10, &buf, &buflen));
    spaced = 0;

    /* Retry interval. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    T(addstr("\t\t\t\t\t", 5, &buf, &buflen));
    T(len = ns_format_ttl(t, buf, buflen));
    addlen(len, &buf, &buflen);
    T(spaced = addtab(len, 16, spaced, &buf, &buflen));
    T(addstr("; retry\n", 8, &buf, &buflen));
    spaced = 0;

    /* Expiry. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    T(addstr("\t\t\t\t\t", 5, &buf, &buflen));
    T(len = ns_format_ttl(t, buf, buflen));
    addlen(len, &buf, &buflen);
    T(spaced = addtab(len, 16, spaced, &buf, &buflen));
    T(addstr("; expiry\n", 9, &buf, &buflen));
    spaced = 0;

    /* Minimum TTL. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    T(addstr("\t\t\t\t\t", 5, &buf, &buflen));
    T(len = ns_format_ttl(t, buf, buflen));
    addlen(len, &buf, &buflen);
    T(addstr(" )", 2, &buf, &buflen));
    T(spaced = addtab(len, 16, spaced, &buf, &buflen));
    T(addstr("; minimum\n", 10, &buf, &buflen));

    break;
      }

  case ns_t_mx:
  case ns_t_afsdb:
  case ns_t_rt: {
    u_int t;

    if (rdlen < NS_INT16SZ)
      goto formerr;

    /* Priority. */
    t = ns_get16(rdata);
    rdata += NS_INT16SZ;
    len = SPRINTF((tmp, "%u ", (unsigned int)t));
    T(addstr(tmp, len, &buf, &buflen));

    /* Target. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));

    break;
      }

  case ns_t_px: {
    u_int t;

    if (rdlen < NS_INT16SZ)
      goto formerr;

    /* Priority. */
    t = ns_get16(rdata);
    rdata += NS_INT16SZ;
    len = SPRINTF((tmp, "%u ", (unsigned int)t));
    T(addstr(tmp, len, &buf, &buflen));

    /* Name1. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    T(addstr(" ", 1, &buf, &buflen));

    /* Name2. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));

    break;
      }

  case ns_t_x25:
    T(len = charstr(rdata, edata, &buf, &buflen));
    if (len == 0)
      goto formerr;
    rdata += len;
    break;

  case ns_t_txt:
    while (rdata < edata) {
      T(len = charstr(rdata, edata, &buf, &buflen));
      if (len == 0)
        goto formerr;
      rdata += len;
      if (rdata < edata)
        T(addstr(" ", 1, &buf, &buflen));
    }
    break;

  case ns_t_nsap: {

    (void) inet_nsap_ntoa((int)rdlen, rdata, t);
    T(addstr(t, strlen(t), &buf, &buflen));
    break;
      }

  case ns_t_aaaa:
    if (rdlen != NS_IN6ADDRSZ)
      goto formerr;
    (void) inet_ntop(AF_INET6, rdata, buf, (socklen_t)buflen);
    addlen(strlen(buf), &buf, &buflen);
    break;

  case ns_t_loc: {
    /* XXX protocol format checking? */
    (void) loc_ntoa(rdata, t);
    T(addstr(t, strlen(t), &buf, &buflen));
    break;
      }

  case ns_t_naptr: {
    u_int order, preference;

    if (rdlen < 2*NS_INT16SZ)
      goto formerr;

    /* Order, Precedence. */
    order = ns_get16(rdata);  rdata += NS_INT16SZ;
    preference = ns_get16(rdata); rdata += NS_INT16SZ;
    len = SPRINTF((t, "%u %u ", (unsigned int)order, (unsigned int)preference));
    T(addstr(t, len, &buf, &buflen));

    /* Flags. */
    T(len = charstr(rdata, edata, &buf, &buflen));
    if (len == 0)
      goto formerr;
    rdata += len;
    T(addstr(" ", 1, &buf, &buflen));

    /* Service. */
    T(len = charstr(rdata, edata, &buf, &buflen));
    if (len == 0)
      goto formerr;
    rdata += len;
    T(addstr(" ", 1, &buf, &buflen));

    /* Regexp. */
    T(len = charstr(rdata, edata, &buf, &buflen));
    if ((ssize_t)len < 0)
      return (-1);
    if (len == 0)
      goto formerr;
    rdata += len;
    T(addstr(" ", 1, &buf, &buflen));

    /* Server. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    break;
      }

  case ns_t_srv: {
    u_int priority, weight, port;

    if (rdlen < NS_INT16SZ*3)
      goto formerr;

    /* Priority, Weight, Port. */
    priority = ns_get16(rdata);  rdata += NS_INT16SZ;
    weight   = ns_get16(rdata);  rdata += NS_INT16SZ;
    port     = ns_get16(rdata);  rdata += NS_INT16SZ;
    len = SPRINTF((t, "%u %u %u ", (unsigned int)priority, (unsigned int)weight, (unsigned int)port));
    T(addstr(t, len, &buf, &buflen));

    /* Server. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    break;
      }

  case ns_t_minfo:
  case ns_t_rp:
    /* Name1. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));
    T(addstr(" ", 1, &buf, &buflen));

    /* Name2. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));

    break;

  case ns_t_wks: {
    int n, lcnt;

    if (rdlen < NS_INT32SZ + 1)
      goto formerr;

    /* Address. */
    (void) inet_ntop(AF_INET, rdata, buf, (socklen_t)buflen);
    addlen(strlen(buf), &buf, &buflen);
    rdata += NS_INADDRSZ;

    /* Protocol. */
    len = SPRINTF((tmp, " %u ( ", (unsigned int)*rdata));
    T(addstr(tmp, len, &buf, &buflen));
    rdata += NS_INT8SZ;

    /* Bit map. */
    n = 0;
    lcnt = 0;
    while (rdata < edata) {
      u_int c = *rdata++;
      do {
        if (c & 0200) {
          if (lcnt == 0) {
            T(addstr("\n\t\t\t\t", 5,
               &buf, &buflen));
            lcnt = 10;
            spaced = 0;
          }
          len = SPRINTF((tmp, "%d ", n));
          T(addstr(tmp, len, &buf, &buflen));
          lcnt--;
        }
        c <<= 1;
      } while (++n & 07);
    }
    T(addstr(")", 1, &buf, &buflen));

    break;
      }

  case ns_t_key: {
    u_int keyflags, protocol, algorithm;
    const char *leader;
    int n;

    if (rdlen < NS_INT16SZ + NS_INT8SZ + NS_INT8SZ)
      goto formerr;

    /* Key flags, Protocol, Algorithm. */
    keyflags = ns_get16(rdata);  rdata += NS_INT16SZ;
    protocol = *rdata++;
    algorithm = *rdata++;
    len = SPRINTF((tmp, "0x%04x %u %u",
             (unsigned int)keyflags, (unsigned int)protocol, (unsigned int)algorithm));
    T(addstr(tmp, len, &buf, &buflen));

    /* Public key data. */
    len = b64_ntop(rdata, edata - rdata,
             base64_key, sizeof base64_key);
    if ((ssize_t)len < 0)
      goto formerr;
    if (len > 15) {
      T(addstr(" (", 2, &buf, &buflen));
      leader = "\n\t\t";
      spaced = 0;
    } else
      leader = " ";
    for (n = 0; n < (int)len; n += 48) {
      T(addstr(leader, strlen(leader), &buf, &buflen));
      T(addstr(base64_key + n, MIN(len - n, 48),
         &buf, &buflen));
    }
    if (len > 15)
      T(addstr(" )", 2, &buf, &buflen));

    break;
      }

  case ns_t_sig: {
    u_int type, algorithm, labels, footprint;
    const char *leader;
    u_long t;
    int n;

    if (rdlen < 22)
      goto formerr;

    /* Type covered, Algorithm, Label count, Original TTL. */
          type = ns_get16(rdata);  rdata += NS_INT16SZ;
    algorithm = *rdata++;
    labels = *rdata++;
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    len = SPRINTF((tmp, " %s %d %lu ",
                   p_type((int)type), (int)algorithm, (unsigned long)t));
    T(addstr(tmp, len, &buf, &buflen));
    if (labels != (u_int)dn_count_labels(name))
      goto formerr;

    /* Signature expiry. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    len = SPRINTF((tmp, "%s ", p_secstodate(t)));
    T(addstr(tmp, len, &buf, &buflen));

    /* Time signed. */
    t = ns_get32(rdata);  rdata += NS_INT32SZ;
    len = SPRINTF((tmp, "%s ", p_secstodate(t)));
    T(addstr(tmp, len, &buf, &buflen));

    /* Signature Footprint. */
    footprint = ns_get16(rdata);  rdata += NS_INT16SZ;
    len = SPRINTF((tmp, "%u ", (unsigned int)footprint));
    T(addstr(tmp, len, &buf, &buflen));

    /* Signer's name. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));

    /* Signature. */
    len = b64_ntop(rdata, edata - rdata,
             base64_key, sizeof base64_key);
    if (len > 15) {
      T(addstr(" (", 2, &buf, &buflen));
      leader = "\n\t\t";
      spaced = 0;
    } else
      leader = " ";
    if ((ssize_t)len < 0)
      goto formerr;
    for (n = 0; n < (int)len; n += 48) {
      T(addstr(leader, strlen(leader), &buf, &buflen));
      T(addstr(base64_key + n, MIN(len - n, 48),
         &buf, &buflen));
    }
    if (len > 15)
      T(addstr(" )", 2, &buf, &buflen));

    break;
      }

  case ns_t_nxt: {
    int n, c;

    /* Next domain name. */
    T(addname(msg, msglen, &rdata, origin, &buf, &buflen));

    /* Type bit map. */
    n = (int)(edata - rdata);
    for (c = 0; c < n*8; c++)
      if (NS_NXT_BIT_ISSET(c, rdata)) {
        len = SPRINTF((tmp, " %s", p_type(c)));
        T(addstr(tmp, len, &buf, &buflen));
      }
    break;
      }

  default:
    comment = "unknown RR type";
    goto hexify;
  }
  return ((int)(buf - obuf));
 formerr:
  comment = "RR format error";
 hexify: {
  int n, m;
  char *p;

  len = SPRINTF((tmp, "\\#(\t\t; %s", comment));
  T(addstr(tmp, len, &buf, &buflen));
  while (rdata < edata) {
    p = tmp;
    p += SPRINTF((p, "\n\t"));
    spaced = 0;
    n = MIN(16, (int)(edata - rdata));
    for (m = 0; m < n; m++)
      p += SPRINTF((p, "%02x ", rdata[m]));
    T(addstr(tmp, (u_int)(p - tmp), &buf, &buflen));
    if (n < 16) {
      T(addstr(")", 1, &buf, &buflen));
      T(addtab((u_int)(p - tmp) + 1, 48, spaced, &buf, &buflen));
    }
    p = tmp;
    p += SPRINTF((p, "; "));
    for (m = 0; m < n; m++)
      *p++ = (isascii(rdata[m]) && isprint(rdata[m]))
        ? rdata[m]
        : '.';
    T(addstr(tmp, (u_int)(p - tmp), &buf, &buflen));
    rdata += n;
  }
  return ((int)(buf - obuf));
    }
}

/* Private. */

/*
 * size_t
 * prune_origin(name, origin)
 *  Find out if the name is at or under the current origin.
 * return:
 *  Number of characters in name before start of origin,
 *  or length of name if origin does not match.
 * notes:
 *  This function should share code with samedomain().
 */
static size_t
prune_origin(const char *name, const char *origin) {
  const char *oname = name;

  while (*name != '\0') {
    if (origin != NULL && strcasecmp(name, origin) == 0)
      return ((size_t)(name - oname) - (name > oname));
    while (*name != '\0') {
      if (*name == '\\') {
        name++;
        /* XXX need to handle \nnn form. */
        if (*name == '\0')
          break;
      } else if (*name == '.') {
        name++;
        break;
      }
      name++;
    }
  }
  return ((size_t)(name - oname));
}

/*
 * int
 * charstr(rdata, edata, buf, buflen)
 *  Format a <character-string> into the presentation buffer.
 * return:
 *  Number of rdata octets consumed
 *  0 for protocol format error
 *  -1 for output buffer error
 * side effects:
 *  buffer is advanced on success.
 */
static int
charstr(const u_char *rdata, const u_char *edata, char **buf, size_t *buflen) {
  const u_char *odata = rdata;
  size_t save_buflen = *buflen;
  char *save_buf = *buf;

  if (addstr("\"", 1, buf, buflen) < 0)
    goto enospc;
  if (rdata < edata) {
    int n = *rdata;

    if (rdata + 1 + n <= edata) {
      rdata++;
      while (n-- > 0) {
        if (strchr("\n\"\\", *rdata) != NULL)
          if (addstr("\\", 1, buf, buflen) < 0)
            goto enospc;
        if (addstr((const char *)rdata, 1,
             buf, buflen) < 0)
          goto enospc;
        rdata++;
      }
    }
  }
  if (addstr("\"", 1, buf, buflen) < 0)
    goto enospc;
  return ((int)(rdata - odata));
 enospc:
  errno = ENOSPC;
  *buf = save_buf;
  *buflen = save_buflen;
  return (-1);
}

static int
addname(const u_char *msg, size_t msglen,
  const u_char **pp, const char *origin,
  char **buf, size_t *buflen)
{
  size_t newlen, save_buflen = *buflen;
  char *save_buf = *buf;
  int n;

  n = dn_expand(msg, msg + msglen, *pp, *buf, (int)(*buflen));
  if (n < 0)
    goto enospc;  /* Guess. */
  newlen = prune_origin(*buf, origin);
  if ((origin == NULL || origin[0] == '\0' || (*buf)[newlen] == '\0') &&
      (newlen == 0 || (*buf)[newlen - 1] != '.')) {
    /* No trailing dot. */
    if (newlen + 2 > *buflen)
      goto enospc;  /* No room for ".\0". */
    (*buf)[newlen++] = '.';
    (*buf)[newlen] = '\0';
  }
  if (newlen == 0) {
    /* Use "@" instead of name. */
    if (newlen + 2 > *buflen)
      goto enospc;        /* No room for "@\0". */
    (*buf)[newlen++] = '@';
    (*buf)[newlen] = '\0';
  }
  *pp += n;
  addlen(newlen, buf, buflen);
  **buf = '\0';
  return ((int)newlen);
 enospc:
  errno = ENOSPC;
  *buf = save_buf;
  *buflen = save_buflen;
  return (-1);
}

static void
addlen(size_t len, char **buf, size_t *buflen) {
  assert(len <= *buflen);
  *buf += len;
  *buflen -= len;
}

static int
addstr(const char *src, size_t len, char **buf, size_t *buflen) {
  if (len > *buflen) {
    errno = ENOSPC;
    return (-1);
  }
  memcpy(*buf, src, len);
  addlen(len, buf, buflen);
  **buf = '\0';
  return (0);
}

static int
addtab(size_t len, size_t target, int spaced, char **buf, size_t *buflen) {
  size_t save_buflen = *buflen;
  char *save_buf = *buf;
  int t;

  if (spaced || len >= target - 1) {
    T(addstr("  ", 2, buf, buflen));
    spaced = 1;
  } else {
    for (t = (int)(target - len - 1) / 8; t >= 0; t--)
      if (addstr("\t", 1, buf, buflen) < 0) {
        *buflen = save_buflen;
        *buf = save_buf;
        return (-1);
      }
    spaced = 0;
  }
  return (spaced);
}
