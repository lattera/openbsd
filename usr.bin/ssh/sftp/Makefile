#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.8.2.1 2005/03/10 17:15:05 brad Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-common.c sftp-glob.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto -ledit -ltermcap
DPADD+=	${LIBCRYPTO}
