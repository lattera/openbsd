# $OpenBSD: src/sys/dev/microcode/siop/Makefile,v 1.1.4.1 2001/05/14 22:25:16 niklas Exp $
# $NetBSD: Makefile,v 1.1 2000/04/21 17:57:01 bouyer Exp $

all: siop.out

PROG=	ncr53cxxx
MKSHARE=no
MAN=

.include <bsd.prog.mk>

regen: siop.out
headers: siop.out

siop.out: siop.ss ${PROG}
	./${PROG} siop.ss -o siop.out

