#	$OpenBSD: src/usr.bin/ssh/ssh-add/Makefile,v 1.17.2.5 2001/09/27 00:16:02 miod Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-add
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-add.1

SRCS=	ssh-add.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+= ${LIBCRYPTO}
