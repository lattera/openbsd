#	$OpenBSD: src/usr.bin/ssh/ssh-keyscan/Makefile,v 1.3.2.2 2001/03/21 18:53:30 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keyscan
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-keyscan.1

SRCS=	ssh-keyscan.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
