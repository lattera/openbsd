#	$OpenBSD: src/sys/arch/i386/isa/pcvt/Util/vttest/Attic/Makefile,v 1.2.6.1 2001/04/18 16:08:20 niklas Exp $

.if ${MACHINE} == "i386"
PROG=		vttest
CFLAGS+=	-traditional -DUSEMYSTTY
SRCS=		main.c esc.c
.else
NOPROG=yes
.endif

MAN1=		vttest.1
MANSUBDIR=	/i386

.include <bsd.prog.mk>
