#	$OpenBSD: src/sys/arch/mvme88k/stand/installboot/Makefile,v 1.4.4.1 2001/04/18 16:11:49 niklas Exp $

.if ${MACHINE} == "mvme88k"
PROG=	installboot
MAN=	installboot.8
MANSUBDIR=/mvme88k

BINDIR=/usr/mdec
DPADD=${LIBUTIL}
LDADD=-lutil
CFLAGS+=-O0
# Need this to work in the miniroot
LDSTATIC=-static
.else
NOPROG=
.endif

.include <bsd.prog.mk>

