/*	$OpenBSD: src/usr.bin/netstat/route.c,v 1.13 1997/06/29 20:18:03 millert Exp $	*/
/*	$NetBSD: route.c,v 1.15 1996/05/07 02:55:06 thorpej Exp $	*/

/*
 * Copyright (c) 1983, 1988, 1993
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
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
 */

#ifndef lint
#if 0
static char sccsid[] = "from: @(#)route.c	8.3 (Berkeley) 3/9/94";
#else
static char *rcsid = "$OpenBSD: src/usr.bin/netstat/route.c,v 1.13 1997/06/29 20:18:03 millert Exp $";
#endif
#endif /* not lint */

#include <sys/param.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/mbuf.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#define _KERNEL
#include <net/route.h>
#undef _KERNEL
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netns/ns.h>

#include <netipx/ipx.h>

#include <sys/sysctl.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef INET
#define INET
#endif

#include <net/encap.h>
#include "netstat.h"

#define kget(p, d) (kread((u_long)(p), (char *)&(d), sizeof (d)))

/*
 * Definitions for showing gateway flags.
 */
struct bits {
	short	b_mask;
	char	b_val;
} bits[] = {
	{ RTF_UP,	'U' },
	{ RTF_GATEWAY,	'G' },
	{ RTF_HOST,	'H' },
	{ RTF_REJECT,	'R' },
	{ RTF_DYNAMIC,	'D' },
	{ RTF_MODIFIED,	'M' },
	{ RTF_DONE,	'd' }, /* Completed -- for routing messages only */
	{ RTF_MASK,	'm' }, /* Mask Present -- for routing messages only */
	{ RTF_CLONING,	'C' },
	{ RTF_XRESOLVE,	'X' },
	{ RTF_LLINFO,	'L' },
	{ RTF_STATIC,	'S' },
	{ RTF_PROTO1,	'1' },
	{ RTF_PROTO2,	'2' },
	{ 0 }
};

static union {
	struct		sockaddr u_sa;
	u_int32_t	u_data[64];
} pt_u;

int	do_rtent = 0;
struct	rtentry rtentry;
struct	radix_node rnode;
struct	radix_mask rmask;

int	NewTree = 0;

static struct sockaddr *kgetsa __P((struct sockaddr *));
static void p_tree __P((struct radix_node *));
static void p_rtnode __P(());
static void ntreestuff __P(());
static void np_rtentry __P((struct rt_msghdr *));
static void p_sockaddr __P((struct sockaddr *, int, int));
static void p_flags __P((int, char *));
static void p_rtentry __P((struct rtentry *));
static void encap_print __P((struct rtentry *));

/*
 * Print routing tables.
 */
void
routepr(rtree)
	u_long rtree;
{
	struct radix_node_head *rnh, head;
	int i;

	printf("Routing tables\n");

	if (Aflag == 0 && NewTree)
		ntreestuff();
	else {
		if (rtree == 0) {
			printf("rt_tables: symbol not in namelist\n");
			return;
		}

		kget(rtree, rt_tables);
		for (i = 0; i <= AF_MAX; i++) {
			if ((rnh = rt_tables[i]) == 0)
				continue;
			kget(rnh, head);
			if (i == AF_UNSPEC) {
				if (Aflag && af == 0) {
					printf("Netmasks:\n");
					p_tree(head.rnh_treetop);
				}
			} else if (af == AF_UNSPEC || af == i) {
				pr_family(i);
				do_rtent = 1;
				if (i != AF_ENCAP)
					pr_rthdr();
				else
					pr_encaphdr();
				p_tree(head.rnh_treetop);
			}
		}
	}
}

/*
 * Print address family header before a section of the routing table.
 */
void
pr_family(af)
	int af;
{
	char *afname;

	switch (af) {
	case AF_INET:
		afname = "Internet";
		break;
	case AF_NS:
		afname = "XNS";
		break;
	case AF_IPX:
		afname = "IPX";
		break;
	case AF_ISO:
		afname = "ISO";
		break;
	case AF_CCITT:
		afname = "X.25";
		break;
	case AF_ENCAP:
		afname = "Encap";
		break;
	default:
		afname = NULL;
		break;
	}
	if (afname)
		printf("\n%s:\n", afname);
	else
		printf("\nProtocol Family %d:\n", af);
}

/* column widths; each followed by one space */
#define	WID_DST		16	/* width of destination column */
#define	WID_GW		18	/* width of gateway column */

/*
 * Print header for routing table columns.
 */
void
pr_rthdr()
{

	if (Aflag)
		printf("%-8.8s ","Address");
	printf("%-*.*s %-*.*s %-6.6s  %6.6s%8.8s %6.6s  %s\n",
		WID_DST, WID_DST, "Destination",
		WID_GW, WID_GW, "Gateway",
		"Flags", "Refs", "Use", "Mtu", "Interface");
}

/*
 * Print header for AF_ENCAP entries.
 */
void
pr_encaphdr()
{
	if (Aflag)
		printf("%-8s ", "Address");
	printf("%-15s %-15s %-5s %-15s %-15s %-5s %-5s %-15s %-8s %s\n",
	    "Source address", "Source mask", "Port", "Dest. address", 
	    "Dest. mask", "Port", "Proto", "Tunnel exit",
	    "SPI", "Use");
}

static struct sockaddr *
kgetsa(dst)
	register struct sockaddr *dst;
{

	kget(dst, pt_u.u_sa);
	if (pt_u.u_sa.sa_len > sizeof (pt_u.u_sa))
		kread((u_long)dst, (char *)pt_u.u_data, pt_u.u_sa.sa_len);
	return (&pt_u.u_sa);
}

static void
p_tree(rn)
	struct radix_node *rn;
{

again:
	kget(rn, rnode);
	if (rnode.rn_b < 0) {
		if (Aflag)
			printf("%-8.8lx ", rn);
		if (rnode.rn_flags & RNF_ROOT) {
			if (Aflag)
				printf("(root node)%s",
				    rnode.rn_dupedkey ? " =>\n" : "\n");
		} else if (do_rtent) {
			kget(rn, rtentry);
			p_rtentry(&rtentry);
			if (Aflag)
				p_rtnode();
		} else {
			p_sockaddr(kgetsa((struct sockaddr *)rnode.rn_key),
			    0, 44);
			putchar('\n');
		}
		if ((rn = rnode.rn_dupedkey))
			goto again;
	} else {
		if (Aflag && do_rtent) {
			printf("%-8.8lx ", rn);
			p_rtnode();
		}
		rn = rnode.rn_r;
		p_tree(rnode.rn_l);
		p_tree(rn);
	}
}

char	nbuf[20];

static void
p_rtnode()
{
	struct radix_mask *rm = rnode.rn_mklist;

	if (rnode.rn_b < 0) {
		if (rnode.rn_mask) {
			printf("\t  mask ");
			p_sockaddr(kgetsa((struct sockaddr *)rnode.rn_mask),
			    0, -1);
		} else if (rm == 0)
			return;
	} else {
		sprintf(nbuf, "(%d)", rnode.rn_b);
		printf("%6.6s %8.8lx : %8.8lx", nbuf, rnode.rn_l, rnode.rn_r);
	}
	while (rm) {
		kget(rm, rmask);
		sprintf(nbuf, " %d refs, ", rmask.rm_refs);
		printf(" mk = %8.8lx {(%d),%s",
			rm, -1 - rmask.rm_b, rmask.rm_refs ? nbuf : " ");
		p_sockaddr(kgetsa((struct sockaddr *)rmask.rm_mask), 0, -1);
		putchar('}');
		if ((rm = rmask.rm_mklist))
			printf(" ->");
	}
	putchar('\n');
}

static void
ntreestuff()
{
	size_t needed;
	int mib[6];
	char *buf, *next, *lim;
	register struct rt_msghdr *rtm;

	mib[0] = CTL_NET;
	mib[1] = PF_ROUTE;
	mib[2] = 0;
	mib[3] = 0;
	mib[4] = NET_RT_DUMP;
	mib[5] = 0;
	if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0) {
		perror("route-sysctl-estimate");
		exit(1);
	}
	if ((buf = malloc(needed)) == 0) {
		printf("out of space\n");
		exit(1);
	}
        if (sysctl(mib, 6, buf, &needed, NULL, 0) < 0) {
		perror("sysctl of routing table");
		exit(1);
	}
	lim = buf + needed;
	for (next = buf; next < lim; next += rtm->rtm_msglen) {
		rtm = (struct rt_msghdr *)next;
		np_rtentry(rtm);
	}
}

static void
np_rtentry(rtm)
	register struct rt_msghdr *rtm;
{
	register struct sockaddr *sa = (struct sockaddr *)(rtm + 1);
#ifdef notdef
	static int masks_done, banner_printed;
#endif
	static int old_af;
	int af = 0, interesting = RTF_UP | RTF_GATEWAY | RTF_HOST;

#ifdef notdef
	/* for the moment, netmasks are skipped over */
	if (!banner_printed) {
		printf("Netmasks:\n");
		banner_printed = 1;
	}
	if (masks_done == 0) {
		if (rtm->rtm_addrs != RTA_DST ) {
			masks_done = 1;
			af = sa->sa_family;
		}
	} else
#endif
		af = sa->sa_family;
	if (af != old_af) {
		pr_family(af);
		old_af = af;
	}
	if (rtm->rtm_addrs == RTA_DST)
		p_sockaddr(sa, 0, 36);
	else {
		p_sockaddr(sa, rtm->rtm_flags, 16);
		if (sa->sa_len == 0)
			sa->sa_len = sizeof(in_addr_t);
		sa = (struct sockaddr *)(sa->sa_len + (char *)sa);
		p_sockaddr(sa, 0, 18);
	}
	p_flags(rtm->rtm_flags & interesting, "%-6.6s ");
	putchar('\n');
}

static void
p_sockaddr(sa, flags, width)
	struct sockaddr *sa;
	int flags, width;
{
	char workbuf[128], *cplim;
	register char *cp = workbuf;

	switch(sa->sa_family) {
	case AF_INET:
	    {
		register struct sockaddr_in *sin = (struct sockaddr_in *)sa;

		cp = (sin->sin_addr.s_addr == 0) ? "default" :
		      ((flags & RTF_HOST) ?
			routename(sin->sin_addr.s_addr) :
			netname(sin->sin_addr.s_addr, INADDR_ANY));
		break;
	    }

	case AF_NS:
		cp = ns_print(sa);
		break;

	case AF_IPX:
		cp = ipx_print(sa);
		break;
		
	case AF_LINK:
	    {
		register struct sockaddr_dl *sdl = (struct sockaddr_dl *)sa;

		if (sdl->sdl_nlen == 0 && sdl->sdl_alen == 0 &&
		    sdl->sdl_slen == 0)
			(void) sprintf(workbuf, "link#%d", sdl->sdl_index);
		else switch (sdl->sdl_type) {
		case IFT_ETHER:
		    {
			register int i;
			register u_char *lla = (u_char *)sdl->sdl_data +
			    sdl->sdl_nlen;

			cplim = "";
			for (i = 0; i < sdl->sdl_alen; i++, lla++) {
				cp += snprintf(cp,
				    workbuf + sizeof (workbuf) - cp,
				    "%s%x", cplim, *lla);
				cplim = ":";
			}
			cp = workbuf;
			break;
		    }
		default:
			cp = link_ntoa(sdl);
			break;
		}
		break;
	    }

	default:
	    {
		register u_char *s = (u_char *)sa->sa_data, *slim;

		slim = sa->sa_len + (u_char *) sa;
		cplim = cp + sizeof(workbuf) - 6;
		cp += sprintf(cp, "(%d)", sa->sa_family);
		while (s < slim && cp < cplim) {
			cp += snprintf(cp, workbuf + sizeof (workbuf) - cp,
			    " %02x", *s++);
			if (s < slim)
				cp += snprintf(cp,
				    workbuf + sizeof (workbuf) - cp,
				    "%02x", *s++);
		}
		cp = workbuf;
	    }
	}
	if (width < 0 )
		printf("%s ", cp);
	else {
		if (nflag)
			printf("%-*s ", width, cp);
		else
			printf("%-*.*s ", width, width, cp);
	}
}

static void
p_flags(f, format)
	register int f;
	char *format;
{
	char name[33], *flags;
	register struct bits *p = bits;

	for (flags = name; p->b_mask; p++)
		if (p->b_mask & f)
			*flags++ = p->b_val;
	*flags = '\0';
	printf(format, name);
}

static void
p_rtentry(rt)
	register struct rtentry *rt;
{
	static struct ifnet ifnet, *lastif;
	struct sockaddr *sa;
	
	sa = kgetsa(rt_key(rt));
	
	if (sa->sa_family == AF_ENCAP) {
		encap_print(rt);
		return;
	}
	
	p_sockaddr(sa, rt->rt_flags, WID_DST);
	p_sockaddr(kgetsa(rt->rt_gateway), RTF_HOST, WID_GW);
	p_flags(rt->rt_flags, "%-6.6s ");
	printf("%6d %8ld ", rt->rt_refcnt, rt->rt_use);
	if (rt->rt_rmx.rmx_mtu)
		printf("%6ld ", rt->rt_rmx.rmx_mtu);
	else
		printf("%6s ", "-");
	if (rt->rt_ifp) {
		if (rt->rt_ifp != lastif) {
			kget(rt->rt_ifp, ifnet);
			lastif = rt->rt_ifp;
		}
		printf(" %.16s%s", ifnet.if_xname,
			rt->rt_nodes[0].rn_dupedkey ? " =>" : "");
	}
	putchar('\n');
}

char *
routename(in)
	in_addr_t in;
{
	register char *cp;
	static char line[MAXHOSTNAMELEN + 1];
	struct hostent *hp;
	static char domain[MAXHOSTNAMELEN + 1];
	static int first = 1;

	if (first) {
		first = 0;
		if (gethostname(domain, MAXHOSTNAMELEN) == 0 &&
		    (cp = strchr(domain, '.')))
			(void) strcpy(domain, cp + 1);
		else
			domain[0] = 0;
	}
	cp = 0;
	if (!nflag) {
		hp = gethostbyaddr((char *)&in, sizeof (struct in_addr),
			AF_INET);
		if (hp) {
			if ((cp = strchr(hp->h_name, '.')) &&
			    !strcmp(cp + 1, domain))
				*cp = 0;
			cp = hp->h_name;
		}
	}
	if (cp)
		strncpy(line, cp, sizeof(line) - 1);
	else {
#define C(x)	((x) & 0xff)
		in = ntohl(in);
		sprintf(line, "%u.%u.%u.%u",
		    C(in >> 24), C(in >> 16), C(in >> 8), C(in));
	}
	return (line);
}

/*
 * Return the name of the network whose address is given.
 * The address is assumed to be that of a net or subnet, not a host.
 */
char *
netname(in, mask)
	in_addr_t in, mask;
{
	char *cp = 0;
	static char line[MAXHOSTNAMELEN + 1];
	struct netent *np = 0;
	in_addr_t net, subnetshift;

	in = ntohl(in);
	mask = ntohl(mask);
	if (!nflag && in != INADDR_ANY) {
		np = getnetbyaddr(in, AF_INET);
		if (np == NULL) {
			if (mask == INADDR_ANY) {
				if (IN_CLASSA(in)) {
					mask = IN_CLASSA_NET;
					subnetshift = 8;
				} else if (IN_CLASSB(in)) {
					mask = IN_CLASSB_NET;
					subnetshift = 8;
				} else {
					mask = IN_CLASSC_NET;
					subnetshift = 4;
				}
				/*
			 	* If there are more bits than the standard mask
			 	* would suggest, subnets must be in use.
			 	* Guess at the subnet mask, assuming reasonable
			 	* width subnet fields.
			 	*/
				while (in &~ mask)
					mask = (int)mask >> subnetshift;
			}
			net = in & mask;
			while ((mask & 1) == 0)
				mask >>= 1, net >>= 1;
			np = getnetbyaddr(net, AF_INET);
		}
		if (np)
			cp = np->n_name;
	}
	if (cp)
		strncpy(line, cp, sizeof(line) - 1);
	else if ((in & 0xffffff) == 0)
		sprintf(line, "%u", C(in >> 24));
	else if ((in & 0xffff) == 0)
		sprintf(line, "%u.%u", C(in >> 24) , C(in >> 16));
	else if ((in & 0xff) == 0)
		sprintf(line, "%u.%u.%u", C(in >> 24), C(in >> 16), C(in >> 8));
	else
		sprintf(line, "%u.%u.%u.%u", C(in >> 24),
			C(in >> 16), C(in >> 8), C(in));
	return (line);
}

/*
 * Print routing statistics
 */
void
rt_stats(off)
	u_long off;
{
	struct rtstat rtstat;

	if (off == 0) {
		printf("rtstat: symbol not in namelist\n");
		return;
	}
	kread(off, (char *)&rtstat, sizeof (rtstat));
	printf("routing:\n");
	printf("\t%u bad routing redirect%s\n",
		rtstat.rts_badredirect, plural(rtstat.rts_badredirect));
	printf("\t%u dynamically created route%s\n",
		rtstat.rts_dynamic, plural(rtstat.rts_dynamic));
	printf("\t%u new gateway%s due to redirects\n",
		rtstat.rts_newgateway, plural(rtstat.rts_newgateway));
	printf("\t%u destination%s found unreachable\n",
		rtstat.rts_unreach, plural(rtstat.rts_unreach));
	printf("\t%u use%s of a wildcard route\n",
		rtstat.rts_wildcard, plural(rtstat.rts_wildcard));
}

short ns_nullh[] = {0,0,0};
short ns_bh[] = {-1,-1,-1};

char *
ns_print(sa)
	register struct sockaddr *sa;
{
	register struct sockaddr_ns *sns = (struct sockaddr_ns*)sa;
	struct ns_addr work;
	union { union ns_net net_e; u_long long_e; } net;
	in_port_t port;
	static char mybuf[50], cport[10], chost[25];
	char *host = "";
	register char *p; register u_char *q;

	work = sns->sns_addr;
	port = ntohs(work.x_port);
	work.x_port = 0;
	net.net_e = work.x_net;
	if (ns_nullhost(work) && net.long_e == 0) {
		if (port ) {
			sprintf(mybuf, "*.%xH", port);
			upHex(mybuf);
		} else
			sprintf(mybuf, "*.*");
		return (mybuf);
	}

	if (bcmp(ns_bh, work.x_host.c_host, 6) == 0) {
		host = "any";
	} else if (bcmp(ns_nullh, work.x_host.c_host, 6) == 0) {
		host = "*";
	} else {
		q = work.x_host.c_host;
		sprintf(chost, "%02x%02x%02x%02x%02x%02xH",
			q[0], q[1], q[2], q[3], q[4], q[5]);
		for (p = chost; *p == '0' && p < chost + 12; p++)
			continue;
		host = p;
	}
	if (port)
		sprintf(cport, ".%xH", htons(port));
	else
		*cport = 0;

	sprintf(mybuf,"%xH.%s%s", ntohl(net.long_e), host, cport);
	upHex(mybuf);
	return(mybuf);
}

char *
ns_phost(sa)
	struct sockaddr *sa;
{
	register struct sockaddr_ns *sns = (struct sockaddr_ns *)sa;
	struct sockaddr_ns work;
	static union ns_net ns_zeronet;
	char *p;

	work = *sns;
	work.sns_addr.x_port = 0;
	work.sns_addr.x_net = ns_zeronet;

	p = ns_print((struct sockaddr *)&work);
	if (strncmp("0H.", p, 3) == 0) p += 3;
	return(p);
}

u_short ipx_nullh[] = {0,0,0};
u_short ipx_bh[] = {0xffff,0xffff,0xffff};

char *
ipx_print(sa)
	register struct sockaddr *sa;
{
	register struct sockaddr_ipx *sipx = (struct sockaddr_ipx*)sa;
	struct ipx_addr work;
	union { union ipx_net net_e; u_long long_e; } net;
	in_port_t port;
	static char mybuf[50], cport[10], chost[25];
	char *host = "";
	register char *q;

	work = sipx->sipx_addr;
	port = ntohs(work.ipx_port);
	work.ipx_port = 0;
	net.net_e = work.ipx_net;
	if (ipx_nullhost(work) && net.long_e == 0) {
		if (port != 0) {
			sprintf(mybuf, "*.%xH", port);
			upHex(mybuf);
		} else
			sprintf(mybuf, "*.*");
		return (mybuf);
	}

	if (bcmp(ipx_bh, work.ipx_host.c_host, 6) == 0) {
		host = "any";
	} else if (bcmp(ipx_nullh, work.ipx_host.c_host, 6) == 0) {
		host = "*";
	} else {
		q = work.ipx_host.c_host;
		sprintf(chost, "%02x:%02x:%02x:%02x:%02x:%02x",
			q[0], q[1], q[2], q[3], q[4], q[5]);
		host = chost;
	}
	if (port)
		sprintf(cport, ".%xH", htons(port));
	else
		*cport = 0;

	sprintf(mybuf,"%xH.%s%s", ntohl(net.long_e), host, cport);
	upHex(mybuf);
	return(mybuf);
}

char *
ipx_phost(sa)
	struct sockaddr *sa;
{
	register struct sockaddr_ipx *sipx = (struct sockaddr_ipx *)sa;
	struct sockaddr_ipx work;
	static union ipx_net ipx_zeronet;
	char *p;

	work = *sipx;
	work.sipx_addr.ipx_port = 0;
	work.sipx_addr.ipx_net = ipx_zeronet;

	p = ipx_print((struct sockaddr *)&work);
	if (strncmp("0H.", p, 3) == 0) p += 3;
	return(p);
}

static void
encap_print(rt)
	register struct rtentry *rt;
{
	struct sockaddr_encap sen1, sen2, sen3;

	bcopy(kgetsa(rt_key(rt)), &sen1, sizeof(sen1));
	bcopy(kgetsa(rt_mask(rt)), &sen2, sizeof(sen2));
	bcopy(kgetsa(rt->rt_gateway), &sen3, sizeof(sen3));

	printf("%-15s ", inet_ntoa(sen1.sen_ip_src));
	printf("%-15s %-5u ", inet_ntoa(sen2.sen_ip_src), sen1.sen_sport);
	printf("%-15s ", inet_ntoa(sen1.sen_ip_dst));
	printf("%-15s %-5u %-5u ", inet_ntoa(sen2.sen_ip_dst),
	    sen1.sen_dport, sen1.sen_proto);
	printf("%-15s %08x %-lu\n", inet_ntoa(sen3.sen_ipsp_dst),
	    ntohl(sen3.sen_ipsp_spi), rt->rt_use);
}

void
upHex(p0)
	char *p0;
{
	register char *p = p0;
	for (; *p; p++) switch (*p) {

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		*p += ('A' - 'a');
	}
}
