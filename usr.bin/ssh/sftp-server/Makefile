#	$OpenBSD: src/usr.bin/ssh/sftp-server/Makefile,v 1.1.4.2 2001/03/12 15:44:19 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp-server
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/libexec
MAN=	sftp-server.8

SRCS=	sftp-server.c sftp-common.c log-server.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
