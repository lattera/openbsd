#	$OpenBSD: src/sys/arch/hppa/stand/mkboot/Makefile,v 1.2.4.2 2003/03/27 23:26:54 niklas Exp $

MAN=	mkboot.8
MANSUBDIR=/hppa

.if ${MACHINE} == "hppa"
PROG=	mkboot
CC=	${HOSTCC}
LDSTATIC=	-static
.else
NOPROG=
.endif

.include <bsd.prog.mk>
