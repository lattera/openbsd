#	$OpenBSD: src/sys/arch/hppa/stand/mkboot/Makefile,v 1.3.8.1 2003/05/19 21:43:13 tedu Exp $

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
