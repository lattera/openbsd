#	$OpenBSD: src/usr.bin/ssh/ssh-keygen/Makefile,v 1.13.2.2 2001/03/12 15:44:20 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-keygen
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-keygen.1

SRCS=	ssh-keygen.c log-client.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto -lutil -lz
DPADD+=	${LIBCRYPTO} ${LIBDES} ${LIBUTIL} ${LIBZ}
