#	$OpenBSD: src/usr.bin/ssh/ssh-keysign/Makefile,v 1.3.2.1 2002/06/22 07:23:42 miod Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keysign
BINOWN=	root

BINMODE?=4555

BINDIR=	/usr/libexec
MAN=	ssh-keysign.8

SRCS=	ssh-keysign.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto -lz
DPADD+=	${LIBCRYPTO} ${LIBZ}
