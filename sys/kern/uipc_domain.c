/*	$OpenBSD: src/sys/kern/uipc_domain.c,v 1.15 2003/06/02 23:28:06 millert Exp $	*/
/*	$NetBSD: uipc_domain.c,v 1.14 1996/02/09 19:00:44 christos Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *
 *	@(#)uipc_domain.c	8.2 (Berkeley) 10/18/93
 */

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/protosw.h>
#include <sys/domain.h>
#include <sys/mbuf.h>
#include <sys/time.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <uvm/uvm_extern.h>
#include <sys/sysctl.h>
#include <sys/timeout.h>

struct	domain *domains;

void	pffasttimo(void *);
void	pfslowtimo(void *);
#if defined (KEY) || defined (IPSEC)
int pfkey_init(void);
#endif /* KEY || IPSEC */

#define	ADDDOMAIN(x)	{ \
	extern struct domain __CONCAT(x,domain); \
	__CONCAT(x,domain.dom_next) = domains; \
	domains = &__CONCAT(x,domain); \
}

void
domaininit()
{
	struct domain *dp;
	struct protosw *pr;
	static struct timeout pffast_timeout;
	static struct timeout pfslow_timeout;

#undef unix
	/*
	 * KAME NOTE: ADDDOMAIN(route) is moved to the last part so that
	 * it will be initialized as the *first* element.  confusing!
	 */
#ifndef lint
	ADDDOMAIN(unix);
#ifdef INET
	ADDDOMAIN(inet);
#endif
#ifdef INET6
	ADDDOMAIN(inet6);
#endif /* INET6 */
#if defined (KEY) || defined (IPSEC)
	pfkey_init();
#endif /* KEY || IPSEC */
#ifdef IPX
	ADDDOMAIN(ipx);
#endif
#ifdef NETATALK
	ADDDOMAIN(atalk);
#endif
#ifdef NS
	ADDDOMAIN(ns);
#endif
#ifdef ISO
	ADDDOMAIN(iso);
#endif
#ifdef CCITT
	ADDDOMAIN(ccitt);
#endif
#ifdef NATM
	ADDDOMAIN(natm);
#endif
#ifdef notdef /* XXXX */
#include "imp.h"
#if NIMP > 0
	ADDDOMAIN(imp);
#endif
#endif
#ifdef IPSEC
#ifdef __KAME__
	ADDDOMAIN(key);
#endif
#endif
	ADDDOMAIN(route);
#endif

	for (dp = domains; dp; dp = dp->dom_next) {
		if (dp->dom_init)
			(*dp->dom_init)();
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_init)
				(*pr->pr_init)();
	}

if (max_linkhdr < 16)		/* XXX */
max_linkhdr = 16;
	max_hdr = max_linkhdr + max_protohdr;
	max_datalen = MHLEN - max_hdr;
	timeout_set(&pffast_timeout, pffasttimo, &pffast_timeout);
	timeout_set(&pfslow_timeout, pfslowtimo, &pfslow_timeout);
	timeout_add(&pffast_timeout, 1);
	timeout_add(&pfslow_timeout, 1);
}

struct protosw *
pffindtype(family, type)
	int family, type;
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		if (dp->dom_family == family)
			goto found;
	return (0);
found:
	for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
		if (pr->pr_type && pr->pr_type == type)
			return (pr);
	return (0);
}

struct protosw *
pffindproto(family, protocol, type)
	int family, protocol, type;
{
	register struct domain *dp;
	register struct protosw *pr;
	struct protosw *maybe = 0;

	if (family == 0)
		return (0);
	for (dp = domains; dp; dp = dp->dom_next)
		if (dp->dom_family == family)
			goto found;
	return (0);
found:
	for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++) {
		if ((pr->pr_protocol == protocol) && (pr->pr_type == type))
			return (pr);

		if (type == SOCK_RAW && pr->pr_type == SOCK_RAW &&
		    pr->pr_protocol == 0 && maybe == (struct protosw *)0)
			maybe = pr;
	}
	return (maybe);
}

int
net_sysctl(name, namelen, oldp, oldlenp, newp, newlen, p)
	int *name;
	u_int namelen;
	void *oldp;
	size_t *oldlenp;
	void *newp;
	size_t newlen;
	struct proc *p;
{
	register struct domain *dp;
	register struct protosw *pr;
	int family, protocol;

	/*
	 * All sysctl names at this level are nonterminal.
	 * PF_KEY: next component is protocol family, and then at least one
	 *	additional component.
	 * usually: next two components are protocol family and protocol
	 *	number, then at least one addition component.
	 */
	if (namelen < 2)
		return (EISDIR);		/* overloaded */
	family = name[0];

	if (family == 0)
		return (0);
	for (dp = domains; dp; dp = dp->dom_next)
		if (dp->dom_family == family)
			goto found;
	return (ENOPROTOOPT);
found:
	switch (family) {
#ifdef IPSEC
#ifdef __KAME__
	case PF_KEY:
		pr = dp->dom_protosw;
		if (pr->pr_sysctl)
			return ((*pr->pr_sysctl)(name + 1, namelen - 1,
				oldp, oldlenp, newp, newlen));
		return (ENOPROTOOPT);
#endif
#endif
	default:
		break;
	}
	if (namelen < 3)
		return (EISDIR);		/* overloaded */
	protocol = name[1];
	for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
		if (pr->pr_protocol == protocol && pr->pr_sysctl)
			return ((*pr->pr_sysctl)(name + 2, namelen - 2,
			    oldp, oldlenp, newp, newlen));
	return (ENOPROTOOPT);
}

void
pfctlinput(cmd, sa)
	int cmd;
	struct sockaddr *sa;
{
	register struct domain *dp;
	register struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_ctlinput)
				(*pr->pr_ctlinput)(cmd, sa, NULL);
}

void
pfslowtimo(arg)
	void *arg;
{
	struct timeout *to = (struct timeout *)arg;
	struct domain *dp;
	struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_slowtimo)
				(*pr->pr_slowtimo)();
	timeout_add(to, hz/2);
}

void
pffasttimo(arg)
	void *arg;
{
	struct timeout *to = (struct timeout *)arg;
	struct domain *dp;
	struct protosw *pr;

	for (dp = domains; dp; dp = dp->dom_next)
		for (pr = dp->dom_protosw; pr < dp->dom_protoswNPROTOSW; pr++)
			if (pr->pr_fasttimo)
				(*pr->pr_fasttimo)();
	timeout_add(to, hz/5);
}
