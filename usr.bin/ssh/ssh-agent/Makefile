#	$OpenBSD: src/usr.bin/ssh/ssh-agent/Makefile,v 1.17.2.1 2001/09/27 19:03:56 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-agent
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-agent.1

SRCS=	ssh-agent.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
