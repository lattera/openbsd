#	$OpenBSD: src/sys/arch/mvme68k/stand/installboot/Makefile,v 1.3.4.2 2004/02/19 10:49:05 niklas Exp $

MAN=	installboot.8
MANSUBDIR=mvme68k

.if ${MACHINE} == "mvme68k"
PROG=	installboot
BINDIR=/usr/mdec
DPADD=${LIBUTIL}
LDADD=-lutil
LDSTATIC=-static
.else
NOPROG=
.endif

.include <bsd.prog.mk>
