#	from @(#)Makefile	8.1 (Berkeley) 6/1/93
#	$Id$

PROG=	kdb_util
SRCS=	kdb_util.c maketime.c
.PATH:	${.CURDIR}/../kdb_edit
DPADD=	${LIBKDB} ${LIBKRB} ${LIBDES}
LDADD=	-lkdb -lkrb -ldes
MAN=	kdb_util.8

.include <bsd.prog.mk>
