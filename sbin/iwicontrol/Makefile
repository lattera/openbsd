# $Id: Makefile,v 1.2 2004/08/19 17:58:43 damien Exp $

.if (${MACHINE} == "i386") || (${MACHINE} == "amd64") || \
    (${MACHINE} == "alpha") || (${MACHINE} == "sparc") || \
    (${MACHINE} == "sparc64") || (${MACHINE_ARCH} == "powerpc") || \
    (${MACHINE} == "cats") || (${MACHINE} == "hppa")
PROG=	iwicontrol
.else
NOPROG=
.endif

MAN=	iwicontrol.8

.include <bsd.prog.mk>
