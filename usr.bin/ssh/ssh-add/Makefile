#	$OpenBSD: src/usr.bin/ssh/ssh-add/Makefile,v 1.17.2.1 2001/02/16 20:13:28 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-add
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-add.1

SRCS=	ssh-add.c log-client.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto -lutil -lz
DPADD+= ${LIBCRYPTO} ${LIBDES} ${LIBUTIL} ${LIBZ}
