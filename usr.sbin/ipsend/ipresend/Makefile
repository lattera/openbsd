#	$Id: Makefile,v 1.1 1996/05/10 21:40:57 dm Exp $

PROG=	ipresend
BINDIR=	/usr/sbin
NOMAN=		
SRCS=	ipresend.c resend.c ip.c sbpf.c sock.c 44arp.c opt.c \
	ipft_ef.c ipft_hx.c ipft_pc.c ipft_sn.c ipft_tx.c ipft_td.c
CFLAGS+=  -DDOSOCKET -I${.CURDIR}/../common -I${.CURDIR}/../../ipftest \
		-I${.CURDIR}/../../../sbin/ipf \
		-I${.CURDIR}/../../../sys/netinet
.PATH:	${.CURDIR}/../common ${.CURDIR}/../../ipftest \
		${.CURDIR}/../../../sbin/ipf

.include <bsd.prog.mk>
