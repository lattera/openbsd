#	$Id: Makefile,v 1.2 1997/11/07 20:20:12 brian Exp $

PROG=	pppctl
SRCS=	pppctl.c
CFLAGS+=-Wall -Wmissing-prototypes
LDADD+= -ledit -ltermcap
DPADD+= ${LIBEDIT} ${LIBTERMCAP}
MAN=	pppctl.8

.include <bsd.prog.mk>
