#	$OpenBSD: src/usr.bin/ssh/ssh-keygen/Makefile,v 1.21.6.1 2003/09/16 21:20:30 brad Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keygen
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-keygen.1

SRCS=	ssh-keygen.c moduli.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
