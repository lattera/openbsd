#	$OpenBSD: src/usr.bin/ssh/ssh-keyscan/Makefile,v 1.2.2.5 2001/09/27 00:16:05 miod Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keyscan
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-keyscan.1

SRCS=	ssh-keyscan.c

.include <bsd.prog.mk>

LDADD+= -lcrypto -lz
DPADD+= ${LIBCRYPTO} ${LIBZ}
