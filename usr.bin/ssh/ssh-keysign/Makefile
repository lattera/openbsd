#	$OpenBSD: src/usr.bin/ssh/ssh-keysign/Makefile,v 1.3.2.2 2002/10/11 14:53:11 miod Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keysign
BINOWN=	root

BINMODE?=4555

BINDIR=	/usr/libexec
MAN=	ssh-keysign.8

SRCS=	ssh-keysign.c readconf.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto -lz
DPADD+=	${LIBCRYPTO} ${LIBZ}
