#	$OpenBSD: src/usr.bin/ssh/sftp/Makefile,v 1.9.6.1 2006/09/30 04:06:51 brad Exp $

.PATH:		${.CURDIR}/..

PROG=	sftp
BINOWN=	root

BINMODE?=555

BINDIR=	/usr/bin
MAN=	sftp.1

SRCS=	sftp.c sftp-client.c sftp-common.c sftp-glob.c

.include <bsd.prog.mk>

LDADD+=	-ledit -ltermcap
