# $Id: Makefile,v 1.1.1.1 2004/07/13 22:02:40 jfb Exp $

.PATH:	${.CURDIR}/..

PROG=cvsd
SRCS= cvsd.c conf.y log.c msg.c sock.c

BINDIR=/usr/sbin
MAN=cvsd.8 cvsd.conf.5

CFLAGS+= -Wall
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wsign-compare

.include <bsd.prog.mk>
