#	$OpenBSD: src/sys/arch/sparc/stand/installboot/Makefile,v 1.4.4.1 2001/05/14 21:37:21 niklas Exp $


MAN=	installboot.8
MANSUBDIR=/sparc

.if ${MACHINE} == "sparc"
BINDIR=	/usr/sbin
PROG=	installboot
.else
NOPROG=
.endif

.include <bsd.prog.mk>
