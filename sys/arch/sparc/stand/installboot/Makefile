#	$OpenBSD: src/sys/arch/sparc/stand/installboot/Makefile,v 1.4.4.2 2001/07/04 10:23:51 niklas Exp $


MAN=	installboot.8
MANSUBDIR=/sparc
LDSTATIC=-static

.if ${MACHINE} == "sparc"
BINDIR=	/usr/sbin
PROG=	installboot
.else
NOPROG=
.endif

.include <bsd.prog.mk>
