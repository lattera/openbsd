#	$OpenBSD: src/usr.bin/ssh/ssh-add/Makefile,v 1.17.2.3 2001/03/21 19:46:33 jason Exp $

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
