#	$OpenBSD: src/sys/arch/mvmeppc/stand/installboot/Attic/Makefile,v 1.1.2.1 2004/06/05 23:10:55 niklas Exp $

.if ${MACHINE} == "mvmeppc"
PROG=	installboot
MAN=	installboot.8
MANSUBDIR=/mvmeppc

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

