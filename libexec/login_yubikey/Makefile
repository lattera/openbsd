# $OpenBSD: src/libexec/login_yubikey/Makefile,v 1.1 2012/01/18 15:49:46 dhill Exp $
#	$Id: Makefile,v 1.1.1.1 2010/03/16 14:13:08 dhartmei Exp $

PROG=	login_yubikey
MAN=	login_yubikey.8
SRCS=	login_yubikey.c yubikey.c
CFLAGS+=-Wall

BINOWN=	root
BINGRP=	auth
BINMODE=2555
BINDIR=	/usr/libexec/auth

.include <bsd.prog.mk>
