# $OpenBSD: src/sys/dev/microcode/siop/Makefile,v 1.1.4.2 2003/03/28 00:38:19 niklas Exp $
# $NetBSD: Makefile,v 1.1 2000/04/21 17:57:01 bouyer Exp $

all: siop.out osiop.out

PROG=	ncr53cxxx
MKSHARE=no
MAN=

.include <bsd.prog.mk>

regen: siop.out osiop.out
headers: siop.out osiop.out

siop.out: siop.ss ${PROG}
	./${PROG} siop.ss -p siop.out

osiop.out: osiop.ss ${PROG}
	./${PROG} osiop.ss -p osiop.out
