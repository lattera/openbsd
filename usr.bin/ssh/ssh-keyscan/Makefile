#	$OpenBSD: src/usr.bin/ssh/ssh-keyscan/Makefile,v 1.3.4.1 2001/09/27 19:03:56 jason Exp $

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
