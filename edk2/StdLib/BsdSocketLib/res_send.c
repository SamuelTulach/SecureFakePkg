/*
 * Copyright (c) 1985, 1989, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Portions copyright (c) 1999, 2000
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
 *    This product includes software developed by the University of
 *    California, Berkeley, Intel Corporation, and its contributors.
 *
 * 4. Neither the name of University, Intel Corporation, or their respective
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS, INTEL CORPORATION AND
 * CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS,
 * INTEL CORPORATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * Portions Copyright (c) 1996 by Internet Software Consortium.
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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)res_send.c  8.1 (Berkeley) 6/4/93";
static char orig_rcsid[] = "From: Id: res_send.c,v 8.20 1998/04/06 23:27:51 halley Exp $";
static char rcsid[] = "$Id: res_send.c,v 1.1.1.1 2003/11/19 01:51:39 kyu3 Exp $";
#endif /* LIBC_SCCS and not lint */

/*
 * Send query to name server and wait for reply.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/uio.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "res_config.h"

#ifndef _ORG_FREEBSD_
#define NOPOLL
#endif

#ifdef NOPOLL           /* libc_r doesn't wrap poll yet() */
#else
#include <poll.h>
static int use_poll = 1;    /* adapt to poll() syscall availability */
                /* 0 = not present, 1 = try it, 2 = exists */
#endif

static int s = -1;      /* socket used for communications */
static int connected = 0;   /* is the socket connected */
static int vc = 0;      /* is the socket a virtual circuit? */
static res_send_qhook Qhook = NULL;
static res_send_rhook Rhook = NULL;


#define CAN_RECONNECT 1

#ifndef DEBUG
#   define Dprint(cond, args) /*empty*/
#   define DprintQ(cond, args, query, size) /*empty*/
#   define Aerror(file, string, error, address) /*empty*/
#   define Perror(file, string, error) /*empty*/
#else
#   define Dprint(cond, args) if (cond) {fprintf args;} else {}
#   define DprintQ(cond, args, query, size) if (cond) {\
            fprintf args;\
            __fp_nquery(query, size, stdout);\
        } else {}

static void
Aerror(
    FILE *file,
    char *string,
    int error,
    struct sockaddr_in address
    )
{
    int save = errno;

    if (_res.options & RES_DEBUG) {
        fprintf(file, "res_send: %s ([%s].%u): %s\n",
            string,
            inet_ntoa(address.sin_addr),
            ntohs(address.sin_port),
            strerror(error));
    }
    errno = save;
}


static void
Perror(
    FILE *file,
    char *string,
    int error
    )
{
    int save = errno;

    if (_res.options & RES_DEBUG) {
        fprintf(file, "res_send: %s: %s\n",
            string, strerror(error));
    }
    errno = save;
}
#endif

void
res_send_setqhook(
    res_send_qhook hook
    )
{

    Qhook = hook;
}

void
res_send_setrhook(
    res_send_rhook hook
    )
{

    Rhook = hook;
}

/* int
 * res_isourserver(ina)
 *  looks up "ina" in _res.ns_addr_list[]
 * returns:
 *  0  : not found
 *  >0 : found
 * author:
 *  paul vixie, 29may94
 */
int
res_isourserver(
    const struct sockaddr_in *inp
    )
{
    struct sockaddr_in ina;
    int ns, ret;

    ina = *inp;
    ret = 0;
    for (ns = 0;  ns < _res.nscount;  ns++) {
        const struct sockaddr_in *srv = &_res.nsaddr_list[ns];

        if (srv->sin_family == ina.sin_family &&
            srv->sin_port == ina.sin_port &&
            (srv->sin_addr.s_addr == INADDR_ANY ||
             srv->sin_addr.s_addr == ina.sin_addr.s_addr)) {
            ret++;
            break;
        }
    }
    return (ret);
}

/* int
 * res_nameinquery(name, type, class, buf, eom)
 *  look for (name,type,class) in the query section of packet (buf,eom)
 * requires:
 *  buf + HFIXEDSZ <= eom
 * returns:
 *  -1 : format error
 *  0  : not found
 *  >0 : found
 * author:
 *  paul vixie, 29may94
 */
int
res_nameinquery(
    const char *name,
    int type,
    int class,
    const u_char *buf,
    const u_char *eom
    )
{
    const u_char *cp = buf + HFIXEDSZ;
    int qdcount = ntohs(((HEADER*)buf)->qdcount);

    while (qdcount-- > 0) {
        char tname[MAXDNAME+1];
        int n, ttype, tclass;

        n = dn_expand(buf, eom, cp, tname, sizeof tname);
        if (n < 0)
            return (-1);
        cp += n;
        if (cp + 2 * INT16SZ > eom)
            return (-1);
        ttype = ns_get16(cp); cp += INT16SZ;
        tclass = ns_get16(cp); cp += INT16SZ;
        if (ttype == type &&
            tclass == class &&
            strcasecmp(tname, name) == 0)
            return (1);
    }
    return (0);
}

/* int
 * res_queriesmatch(buf1, eom1, buf2, eom2)
 *  is there a 1:1 mapping of (name,type,class)
 *  in (buf1,eom1) and (buf2,eom2)?
 * returns:
 *  -1 : format error
 *  0  : not a 1:1 mapping
 *  >0 : is a 1:1 mapping
 * author:
 *  paul vixie, 29may94
 */
int
res_queriesmatch(
    const u_char *buf1,
    const u_char *eom1,
    const u_char *buf2,
    const u_char *eom2
    )
{
    const u_char *cp = buf1 + HFIXEDSZ;
    int qdcount = ntohs(((HEADER*)buf1)->qdcount);

    if (buf1 + HFIXEDSZ > eom1 || buf2 + HFIXEDSZ > eom2)
        return (-1);

    /*
     * Only header section present in replies to
     * dynamic update packets.
     */
    if ( (((HEADER *)buf1)->opcode == ns_o_update) &&
         (((HEADER *)buf2)->opcode == ns_o_update) )
        return (1);

    if (qdcount != ntohs(((HEADER*)buf2)->qdcount))
        return (0);
    while (qdcount-- > 0) {
        char tname[MAXDNAME+1];
        int n, ttype, tclass;

        n = dn_expand(buf1, eom1, cp, tname, sizeof tname);
        if (n < 0)
            return (-1);
        cp += n;
        if (cp + 2 * INT16SZ > eom1)
            return (-1);
        ttype = ns_get16(cp);   cp += INT16SZ;
        tclass = ns_get16(cp); cp += INT16SZ;
        if (!res_nameinquery(tname, ttype, tclass, buf2, eom2))
            return (0);
    }
    return (1);
}

int
res_send(
    const u_char *buf,
    int buflen,
    u_char *ans,
    int anssiz
    )
{
    HEADER *hp = (HEADER *) buf;
    HEADER *anhp = (HEADER *) ans;
    int gotsomewhere, connreset, terrno, try, v_circuit, resplen, ns;
    ssize_t n;
    u_int32_t badns;    /* XXX NSMAX can't exceed #/bits in this variable */

    if ((_res.options & RES_INIT) == 0 && res_init() == -1) {
        /* errno should have been set by res_init() in this case. */
        return (-1);
    }
    if (anssiz < HFIXEDSZ) {
        errno = EINVAL;
        return (-1);
    }
    DprintQ((_res.options & RES_DEBUG) || (_res.pfcode & RES_PRF_QUERY),
        (stdout, ";; res_send()\n"), buf, buflen);
    v_circuit = (_res.options & RES_USEVC) || buflen > PACKETSZ;
    gotsomewhere = 0;
    connreset = 0;
    terrno = ETIMEDOUT;
    badns = 0;

    /*
     * Send request, RETRY times, or until successful
     */
    for (try = 0; try < _res.retry; try++) {
        for (ns = 0; ns < _res.nscount; ns++) {
        struct sockaddr_in *nsap = &_res.nsaddr_list[ns];
    same_ns:
        if (badns & (1 << ns)) {
            res_close();
            goto next_ns;
        }

        if (Qhook) {
            int done = 0, loops = 0;

            do {
                res_sendhookact act;

                act = (*Qhook)(&nsap, &buf, &buflen,
                           ans, anssiz, &resplen);
                switch (act) {
                case res_goahead:
                    done = 1;
                    break;
                case res_nextns:
                    res_close();
                    goto next_ns;
                case res_done:
                    return (resplen);
                case res_modified:
                    /* give the hook another try */
                    if (++loops < 42) /*doug adams*/
                        break;
                    /*FALLTHROUGH*/
                case res_error:
                    /*FALLTHROUGH*/
                default:
                    return (-1);
                }
            } while (!done);
        }

        Dprint(_res.options & RES_DEBUG,
               (stdout, ";; Querying server (# %d) address = %s\n",
            ns + 1, inet_ntoa(nsap->sin_addr)));

        if (v_circuit) {
            int truncated;
            struct iovec iov[2];
            u_short len;
            u_char *cp;

            /*
             * Use virtual circuit;
             * at most one attempt per server.
             */
            try = _res.retry;
            truncated = 0;
            if (s < 0 || !vc || hp->opcode == ns_o_update) {
                if (s >= 0)
                    res_close();

                s = socket(PF_INET, SOCK_STREAM, 0);
                if (s < 0) {
                    terrno = errno;
                    Perror(stderr, "socket(vc)", errno);
                    return (-1);
                }
                errno = 0;
                nsap->sin_len = sizeof ( *nsap );
                if (connect(s, (struct sockaddr *)nsap,
                        sizeof *nsap) < 0) {
                    terrno = errno;
                    Aerror(stderr, "connect/vc",
                           errno, *nsap);
                    badns |= (1 << ns);
                    res_close();
                    goto next_ns;
                }
                vc = 1;
            }
            /*
             * Send length & message
             */
            putshort((u_short)buflen, (u_char*)&len);
            iov[0].iov_base = (caddr_t)&len;
            iov[0].iov_len = INT16SZ;
            iov[1].iov_base = (caddr_t)buf;
            iov[1].iov_len = buflen;
            if (writev(s, iov, 2) != (INT16SZ + buflen)) {
                terrno = errno;
                Perror(stderr, "write failed", errno);
                badns |= (1 << ns);
                res_close();
                goto next_ns;
            }
            /*
             * Receive length & response
             */
read_len:
            cp = ans;
            len = INT16SZ;
            while ((n = read(s, (char *)cp, (int)len)) > 0) {
                cp += n;
                len = (u_short)( len - n );
                if (len <= 0)
                    break;
            }
            if (n <= 0) {
                terrno = errno;
                Perror(stderr, "read failed", errno);
                res_close();
                /*
                 * A long running process might get its TCP
                 * connection reset if the remote server was
                 * restarted.  Requery the server instead of
                 * trying a new one.  When there is only one
                 * server, this means that a query might work
                 * instead of failing.  We only allow one reset
                 * per query to prevent looping.
                 */
                if (terrno == ECONNRESET && !connreset) {
                    connreset = 1;
                    res_close();
                    goto same_ns;
                }
                res_close();
                goto next_ns;
            }
            resplen = ns_get16(ans);
            if (resplen > anssiz) {
                Dprint(_res.options & RES_DEBUG,
                       (stdout, ";; response truncated\n")
                       );
                truncated = 1;
                len = (ushort)anssiz;
            } else
                len = (ushort)resplen;
            if (len < HFIXEDSZ) {
                /*
                 * Undersized message.
                 */
                Dprint(_res.options & RES_DEBUG,
                       (stdout, ";; undersized: %d\n", len));
                terrno = EMSGSIZE;
                badns |= (1 << ns);
                res_close();
                goto next_ns;
            }
            cp = ans;
            while (len != 0 &&
                   (n = read(s, (char *)cp, (int)len)) > 0) {
                cp += n;
                len = (u_short)( len - n );
            }
            if (n <= 0) {
                terrno = errno;
                Perror(stderr, "read(vc)", errno);
                res_close();
                goto next_ns;
            }
            if (truncated) {
                /*
                 * Flush rest of answer
                 * so connection stays in synch.
                 */
                anhp->tc = 1;
                len = (ushort)( resplen - anssiz );
                while (len != 0) {
                    char junk[PACKETSZ];

                    n = (len > sizeof(junk)
                         ? sizeof(junk)
                         : len);
                    if ((n = read(s, junk, n)) > 0)
                        len = (u_short)( len - n );
                    else
                        break;
                }
            }
            /*
             * The calling applicating has bailed out of
             * a previous call and failed to arrange to have
             * the circuit closed or the server has got
             * itself confused. Anyway drop the packet and
             * wait for the correct one.
             */
            if (hp->id != anhp->id) {
                DprintQ((_res.options & RES_DEBUG) ||
                    (_res.pfcode & RES_PRF_REPLY),
                    (stdout, ";; old answer (unexpected):\n"),
                    ans, (resplen>anssiz)?anssiz:resplen);
                goto read_len;
            }
        } else {
            /*
             * Use datagrams.
             */
#ifndef NOPOLL
            struct pollfd pfd;
            int msec;
#endif
            struct timeval timeout;
            fd_set dsmask, *dsmaskp;
            int dsmasklen;
            struct sockaddr_in from;
            int fromlen;

            if ((s < 0) || vc) {
                if (vc)
                    res_close();
                s = socket(PF_INET, SOCK_DGRAM, 0);
                if (s < 0) {
#ifndef CAN_RECONNECT
 bad_dg_sock:
#endif
                    terrno = errno;
                    Perror(stderr, "socket(dg)", errno);
                    return (-1);
                }
                connected = 0;
            }
#ifndef CANNOT_CONNECT_DGRAM
            /*
             * On a 4.3BSD+ machine (client and server,
             * actually), sending to a nameserver datagram
             * port with no nameserver will cause an
             * ICMP port unreachable message to be returned.
             * If our datagram socket is "connected" to the
             * server, we get an ECONNREFUSED error on the next
             * socket operation, and select returns if the
             * error message is received.  We can thus detect
             * the absence of a nameserver without timing out.
             * If we have sent queries to at least two servers,
             * however, we don't want to remain connected,
             * as we wish to receive answers from the first
             * server to respond.
             */
            if (_res.nscount == 1 || (try == 0 && ns == 0)) {
                /*
                 * Connect only if we are sure we won't
                 * receive a response from another server.
                 */
                if (!connected) {
                    nsap->sin_len = sizeof ( *nsap );
                    if (connect(s, (struct sockaddr *)nsap,
                            sizeof *nsap
                            ) < 0) {
                        Aerror(stderr,
                               "connect(dg)",
                               errno, *nsap);
                        badns |= (1 << ns);
                        res_close();
                        goto next_ns;
                    }
                    connected = 1;
                }
                if (send(s, (char*)buf, buflen, 0) != buflen) {
                    Perror(stderr, "send", errno);
                    badns |= (1 << ns);
                    res_close();
                    goto next_ns;
                }
            } else {
                /*
                 * Disconnect if we want to listen
                 * for responses from more than one server.
                 */
                if (connected) {
#ifdef CAN_RECONNECT
                    struct sockaddr_in no_addr;

                    no_addr.sin_family = AF_INET;
                    no_addr.sin_addr.s_addr = INADDR_ANY;
                    no_addr.sin_port = 0;
                    (void) connect(s,
                               (struct sockaddr *)
                                &no_addr,
                               sizeof no_addr);
#else
                    int s1 = socket(PF_INET, SOCK_DGRAM,0);
                    if (s1 < 0)
                        goto bad_dg_sock;
                    (void) dup2(s1, s);
                    (void) close(s1);
                    Dprint(_res.options & RES_DEBUG,
                        (stdout, ";; new DG socket\n"))
#endif /* CAN_RECONNECT */
                    connected = 0;
                    errno = 0;
                }
#endif /* !CANNOT_CONNECT_DGRAM */
                if (sendto(s, (char*)buf, buflen, 0,
                       (struct sockaddr *)nsap,
                       sizeof *nsap)
                    != buflen) {
                    Aerror(stderr, "sendto", errno, *nsap);
                    badns |= (1 << ns);
                    res_close();
                    goto next_ns;
                }
#ifndef CANNOT_CONNECT_DGRAM
            }
#endif /* !CANNOT_CONNECT_DGRAM */

            /*
             * Wait for reply
             */
#ifndef NOPOLL
    othersyscall:
            if (use_poll) {
                msec = (_res.retrans << try) * 1000;
                if (try > 0)
                    msec /= _res.nscount;
                if (msec <= 0)
                    msec = 1000;
            } else {
#endif
                timeout.tv_sec = (_res.retrans << try);
                if (try > 0)
                    timeout.tv_sec /= _res.nscount;
                if ((long) timeout.tv_sec <= 0)
                    timeout.tv_sec = 1;
                timeout.tv_usec = 0;
#ifndef NOPOLL
            }
#endif
    wait:
            if (s < 0) {
                Perror(stderr, "s out-of-bounds", EMFILE);
                res_close();
                goto next_ns;
            }
#ifndef NOPOLL
            if (use_poll) {
                struct sigaction sa, osa;
                int sigsys_installed = 0;

                pfd.fd = s;
                pfd.events = POLLIN;
                if (use_poll == 1) {
                    bzero(&sa, sizeof(sa));
                    sa.sa_handler = SIG_IGN;
                    if (sigaction(SIGSYS, &sa, &osa) >= 0)
                        sigsys_installed = 1;
                }
                n = poll(&pfd, 1, msec);
                if (sigsys_installed == 1) {
                    int oerrno = errno;
                    sigaction(SIGSYS, &osa, NULL);
                    errno = oerrno;
                }
                /* XXX why does nosys() return EINVAL? */
                if (n < 0 && (errno == ENOSYS ||
                    errno == EINVAL)) {
                    use_poll = 0;
                    goto othersyscall;
                } else if (use_poll == 1)
                    use_poll = 2;
                if (n < 0) {
                    if (errno == EINTR)
                        goto wait;
                    Perror(stderr, "poll", errno);
                    res_close();
                    goto next_ns;
                }
            } else {
#endif
                dsmasklen = howmany(s + 1, NFDBITS) *
                        sizeof(fd_mask);
                if (dsmasklen > sizeof(fd_set)) {
                    dsmaskp = (fd_set *)malloc(dsmasklen);
                    if (dsmaskp == NULL) {
                        res_close();
                        goto next_ns;
                    }
                } else
                    dsmaskp = &dsmask;
                /* only zero what we need */
                memset((char *)dsmaskp, 0, dsmasklen);
                FD_SET(s, dsmaskp);
                n = select(s + 1, dsmaskp, (fd_set *)NULL,
                       (fd_set *)NULL, &timeout);
                if (dsmaskp != &dsmask)
                    free(dsmaskp);
                if (n < 0) {
                    if (errno == EINTR)
                        goto wait;
                    Perror(stderr, "select", errno);
                    res_close();
                    goto next_ns;
                }
#ifndef NOPOLL
            }
#endif

            if (n == 0) {
                /*
                 * timeout
                 */
                Dprint(_res.options & RES_DEBUG,
                       (stdout, ";; timeout\n"));
                gotsomewhere = 1;
                res_close();
                goto next_ns;
            }
            errno = 0;
            fromlen = sizeof(struct sockaddr_in);
            resplen = (int)recvfrom(s, (char*)ans, anssiz, 0,
                       (struct sockaddr *)&from, (socklen_t *)&fromlen);
            if (resplen <= 0) {
                Perror(stderr, "recvfrom", errno);
                res_close();
                goto next_ns;
            }
            gotsomewhere = 1;
            if (resplen < HFIXEDSZ) {
                /*
                 * Undersized message.
                 */
                Dprint(_res.options & RES_DEBUG,
                       (stdout, ";; undersized: %d\n",
                    resplen));
                terrno = EMSGSIZE;
                badns |= (1 << ns);
                res_close();
                goto next_ns;
            }
            if (hp->id != anhp->id) {
                /*
                 * response from old query, ignore it.
                 * XXX - potential security hazard could
                 *   be detected here.
                 */
                DprintQ((_res.options & RES_DEBUG) ||
                    (_res.pfcode & RES_PRF_REPLY),
                    (stdout, ";; old answer:\n"),
                    ans, (resplen>anssiz)?anssiz:resplen);
                goto wait;
            }
#ifdef CHECK_SRVR_ADDR
            if (!(_res.options & RES_INSECURE1) &&
                !res_isourserver(&from)) {
                /*
                 * response from wrong server? ignore it.
                 * XXX - potential security hazard could
                 *   be detected here.
                 */
                DprintQ((_res.options & RES_DEBUG) ||
                    (_res.pfcode & RES_PRF_REPLY),
                    (stdout, ";; not our server:\n"),
                    ans, (resplen>anssiz)?anssiz:resplen);
                goto wait;
            }
#endif
            if (!(_res.options & RES_INSECURE2) &&
                !res_queriesmatch(buf, buf + buflen,
                          ans, ans + anssiz)) {
                /*
                 * response contains wrong query? ignore it.
                 * XXX - potential security hazard could
                 *   be detected here.
                 */
                DprintQ((_res.options & RES_DEBUG) ||
                    (_res.pfcode & RES_PRF_REPLY),
                    (stdout, ";; wrong query name:\n"),
                    ans, (resplen>anssiz)?anssiz:resplen);
                goto wait;
            }
            if (anhp->rcode == SERVFAIL ||
                anhp->rcode == NOTIMP ||
                anhp->rcode == REFUSED) {
                DprintQ(_res.options & RES_DEBUG,
                    (stdout, "server rejected query:\n"),
                    ans, (resplen>anssiz)?anssiz:resplen);
                badns |= (1 << ns);
                res_close();
                /* don't retry if called from dig */
                if (!_res.pfcode)
                    goto next_ns;
            }
            if (!(_res.options & RES_IGNTC) && anhp->tc) {
                /*
                 * get rest of answer;
                 * use TCP with same server.
                 */
                Dprint(_res.options & RES_DEBUG,
                       (stdout, ";; truncated answer\n"));
                v_circuit = 1;
                res_close();
                goto same_ns;
            }
        } /*if vc/dg*/
        Dprint((_res.options & RES_DEBUG) ||
               ((_res.pfcode & RES_PRF_REPLY) &&
            (_res.pfcode & RES_PRF_HEAD1)),
               (stdout, ";; got answer:\n"));
        if((_res.options & RES_DEBUG) ||
            (_res.pfcode & RES_PRF_REPLY)) {
            __fp_nquery(ans, (resplen>anssiz)?anssiz:resplen, stdout);
        }
        /*
         * If using virtual circuits, we assume that the first server
         * is preferred over the rest (i.e. it is on the local
         * machine) and only keep that one open.
         * If we have temporarily opened a virtual circuit,
         * or if we haven't been asked to keep a socket open,
         * close the socket.
         */
        if ((v_circuit && (!(_res.options & RES_USEVC) || ns != 0)) ||
            !(_res.options & RES_STAYOPEN)) {
            res_close();
        }
        if (Rhook) {
            int done = 0, loops = 0;

            do {
                res_sendhookact act;

                act = (*Rhook)(nsap, buf, buflen,
                           ans, anssiz, &resplen);
                switch (act) {
                case res_goahead:
                case res_done:
                    done = 1;
                    break;
                case res_nextns:
                    res_close();
                    goto next_ns;
                case res_modified:
                    /* give the hook another try */
                    if (++loops < 42) /*doug adams*/
                        break;
                    /*FALLTHROUGH*/
                case res_error:
                    /*FALLTHROUGH*/
                default:
                    return (-1);
                }
            } while (!done);

        }
        return (resplen);
    next_ns: ;
       } /*foreach ns*/
    } /*foreach retry*/
    res_close();
    if (!v_circuit) {
        if (!gotsomewhere)
            errno = ECONNREFUSED;   /* no nameservers found */
        else
            errno = ETIMEDOUT;  /* no answer obtained */
    } else
        errno = terrno;
    return (-1);
}

/*
 * This routine is for closing the socket if a virtual circuit is used and
 * the program wants to close it.  This provides support for endhostent()
 * which expects to close the socket.
 *
 * This routine is not expected to be user visible.
 */
void
res_close()
{
    if (s >= 0) {
        (void) close(s);
        s = -1;
        connected = 0;
        vc = 0;
    }
}
