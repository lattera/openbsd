#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.4.2.1 2001/09/27 19:03:56 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-int.c sftp-common.c sftp-glob.c misc.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}

