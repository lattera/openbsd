#	from @(#)Makefile	8.1 (Berkeley) 6/1/93
#	$Id: Makefile,v 1.2 1997/11/28 12:48:48 art Exp $

PROG=	klist
SRCS=	klist.c
.PATH:	${.CURDIR}/../roken
SRCS+=	parse_time.c parse_units.c
DPADD=	${LIBKRB} ${LIBDES}
LDADD=	-lkrb -ldes -lkafs
BINDIR=	/usr/bin
CFLAGS+= -I${.CURDIR}/../roken

.include <bsd.prog.mk>
