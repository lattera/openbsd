#	$Id$

PROG=	ipsend
BINDIR=	/usr/sbin
NOMAN=		
SRCS=	ipsend.c ip.c ipsopt.c sbpf.c sock.c 44arp.c
CFLAGS+=  -DDOSOCKET -I${.CURDIR}/../common -I${.CURDIR}/../../ipftest
.PATH:	${.CURDIR}/../common

.include <bsd.prog.mk>
