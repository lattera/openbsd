#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.7.2.1 2004/03/04 18:18:17 brad Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-common.c sftp-glob.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}

