#	$OpenBSD: src/sys/arch/mvme88k/stand/installboot/Makefile,v 1.4.4.2 2004/02/19 10:49:09 niklas Exp $

MAN=	installboot.8
MANSUBDIR=mvme88k

.if ${MACHINE} == "mvme88k"
PROG=	installboot
BINDIR=/usr/mdec
DPADD=${LIBUTIL}
LDADD=-lutil
# Need this to work in the miniroot
LDSTATIC=-static
.else
NOPROG=
.endif

.include <bsd.prog.mk>

