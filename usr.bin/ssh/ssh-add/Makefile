#	$OpenBSD: src/usr.bin/ssh/ssh-add/Makefile,v 1.16.2.2 2001/03/12 15:44:20 jason Exp $

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
