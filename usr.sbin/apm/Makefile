# $Id: Makefile,v 1.1 1996/03/18 05:45:09 jtk Exp $

SRCS=	apm.c apmsubr.c
#LDADD+=	-lutil

.PATH:	${.CURDIR}/../apmd

CFLAGS+=	-g  -O2 -Wmissing-prototypes -Wall -I${.CURDIR}/../apmd
PROG=	apm
MAN=	apm.8
MLINKS=         apm.8 zzz.8
LINKS=          ${BINDIR}/apm ${BINDIR}/zzz

.include <bsd.prog.mk>
# DO NOT DELETE THIS LINE -- make depend depends on it.
