#	$OpenBSD: src/usr.bin/ssh/ssh-keyscan/Makefile,v 1.2.2.1 2001/02/16 20:13:30 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keyscan
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-keyscan.1

SRCS=	ssh-keyscan.c log-client.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
