#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.5.6.1 2003/04/01 00:12:14 margarida Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-int.c sftp-common.c sftp-glob.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}

