#	$OpenBSD: src/usr.bin/ssh/ssh-add/Makefile,v 1.17.2.4 2001/05/07 21:09:41 jason Exp $

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
