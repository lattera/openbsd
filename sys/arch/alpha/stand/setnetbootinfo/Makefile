#	$OpenBSD: src/sys/arch/alpha/stand/setnetbootinfo/Makefile,v 1.1.14.1 2001/04/18 16:01:45 niklas Exp $
#	$NetBSD: Makefile,v 1.4 1997/04/06 08:41:36 cgd Exp $

.PATH:	${.CURDIR}/..

MAN=	setnetbootinfo.8
MANSUBDIR=/alpha

.if ${MACHINE} == "alpha"
PROG=	setnetbootinfo
BINDIR=	/usr/mdec

CFLAGS+= -I${.CURDIR}/..
LDSTATIC=-static
.else
NOPROG=
.endif

.include <bsd.prog.mk>
