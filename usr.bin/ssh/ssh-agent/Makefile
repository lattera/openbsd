#	$OpenBSD: src/usr.bin/ssh/ssh-agent/Makefile,v 1.21.4.1 2002/10/11 14:51:57 miod Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh-agent
BINOWN=	root
#BINGRP=	_sshagnt

BINMODE?=555

BINDIR=	/usr/bin
MAN=	ssh-agent.1

SRCS=	ssh-agent.c

.include <bsd.prog.mk>

LDADD+=	-lcrypto
DPADD+=	${LIBCRYPTO}
