#	$OpenBSD: src/sys/arch/i386/stand/installboot/Makefile,v 1.5.12.1 2001/04/18 16:08:41 niklas Exp $

MAN=	installboot.8

.if ${MACHINE} == "i386"
PROG=	installboot

LDADD=	-lutil
DPADD=	${LIBUTIL}
# Need this to work in the miniroot
LDSTATIC=	-static

SADIR=	${.CURDIR}/..
.else
NOPROG=
.endif

.include <bsd.prog.mk>
