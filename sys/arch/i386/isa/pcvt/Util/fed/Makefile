#	$OpenBSD: src/sys/arch/i386/isa/pcvt/Util/fed/Attic/Makefile,v 1.4.6.1 2001/04/18 16:08:07 niklas Exp $

MAN1=	fed.1
MANSUBDIR=/i386

.if ${MACHINE} == "i386"
PROG=	fed
SRCS=	fed.c select.c edit.c misc.c
LDADD=	-lcurses
.else
NOPROG=	yes
.endif

.include <bsd.prog.mk>
