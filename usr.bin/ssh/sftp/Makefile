#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.2.2.1 2001/03/12 15:44:19 jason Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-int.c sftp-common.c log-client.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}

