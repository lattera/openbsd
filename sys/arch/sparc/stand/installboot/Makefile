#	$OpenBSD: src/sys/arch/sparc/stand/installboot/Makefile,v 1.4.4.3 2004/02/19 10:49:59 niklas Exp $


MAN=	installboot.8
MANSUBDIR=sparc
LDSTATIC=-static

.if ${MACHINE} == "sparc"
BINDIR=	/usr/sbin
PROG=	installboot
.else
NOPROG=
.endif

.include <bsd.prog.mk>
