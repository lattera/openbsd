#	$OpenBSD: src/usr.bin/ssh/sftp-server/Makefile,v 1.1.4.3 2001/03/21 18:53:26 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp-server
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/libexec
MAN=	sftp-server.8

SRCS=	sftp-server.c sftp-common.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
