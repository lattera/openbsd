#	$OpenBSD: src/usr.bin/ssh/ssh-agent/Makefile,v 1.13.2.3 2001/03/21 18:53:28 jason Exp $

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
