#	$Id: Makefile,v 1.1 1998/08/31 00:22:30 brian Exp $

PROG=	pppctl
SRCS=	pppctl.c
MAN=	pppctl.8
CFLAGS+=-Wall
LDADD+= -ledit -ltermcap
DPADD+= ${LIBEDIT} ${LIBTERMCAP}

.include <bsd.prog.mk>
